// #define for header added for redundancy sake.

// NOTE: Ideally table functions would be on their own.
//       But due to limitations of scope they are not.
#include "finance.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// - Helpers -

int is_close(double n1, double n2) {return fabs(n1 - n2) < THRESHOLD;}

int printf_currency(double value, const char* currency_str) {
    if (strlen(currency_str) > 3) {
        return CURRENCY_CODE_IS_TOO_LONG;
    }

    printf("%s %.2lf", currency_str, value);
    return NO_ERROR;
}

int _count_file_lines(const char* file_path) {
    int count = 0;
    char buf[4096];
    
    FILE *file = fopen(file_path, "r");
    
    if (file == NULL) return -1;
    
    while (fgets(buf, sizeof(buf), file) != NULL) count++;

    fclose(file);

    return count;
}

// Prints Schedule table header
void _print_schedule_header() {
    printf("| %-5s| %-19s| %-19s| %-19s| %-19s|\n",
    "Month","Installment","Interest","Amortization","Remaining");
    
    for (int i = 0; i < LINE_WIDTH; i++) putchar('-');
    
    printf("\n");
}

// Prints a single schedule row
void _print_schedule_row(AmortizationRow sched_row) {
    printf("| %-5d| %-19.2lf| %-19.2lf| %-19.2lf| %-19.2lf|\n", 
        sched_row.month,
        sched_row.installment,
        sched_row.interest,
        sched_row.amortization,
        sched_row.remaining_balance
    );
}

// Prints Schedule table footer
void _print_schedule_footer(int table_length) {
    for (int i = 0; i < LINE_WIDTH; i++) putchar('_');
    printf("\nLength: %d\n\n", table_length);
}

void print_schedule_table(AmortizationTable sched_table) {    
    _print_schedule_header();
    for (int _m = 0; _m < sched_table.length; _m++) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    _print_schedule_footer(sched_table.length);
}

void head_schedule_table(AmortizationTable sched_table) {
    _print_schedule_header();
    for (int _m = 0; (_m < 5) && (_m < sched_table.length); _m++) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}

void tail_schedule_table(AmortizationTable sched_table) {
    _print_schedule_header();
    for (int _m = sched_table.length - 1; (_m >= 0) && (_m >= (sched_table.length - 5)); _m--) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}

enum errors_code check_variables(double principal, double rate, int periods) {
    enum errors_code error_code = NO_ERROR;

    if (principal < 0)  error_code += NEG_PRINCIPAL;

    if (periods <= 0) error_code += NEG_PERIODS;

    if (rate < 0) error_code += NEG_RATE;

    return error_code;
}

// - Financial functions --
double simple_interest(double principal, double rate, int periods) {
    enum errors_code valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != NO_ERROR) return NAN;

    return principal * rate * periods;
}

double simple_interest_amount(double principal, double rate, int periods) {
    enum errors_code valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != NO_ERROR) return NAN;

    return simple_interest(principal, rate, periods) + principal;
}

double compound_interest_amount(double principal, double rate, int periods) {
    enum errors_code valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != NO_ERROR) return NAN;

    return principal * (pow((1 + rate), periods));
}

double compound_interest(double principal, double rate, int periods) {
    // Error checked on 'compound_interest_amount'
    return compound_interest_amount(principal, rate, periods) - principal;
}

double fixed_installment(double principal, double monthly_rate, int months) {
    enum errors_code valid_variables = check_variables(principal, monthly_rate, months);
    
    if (valid_variables != NO_ERROR) return NAN;

    if (!is_close(monthly_rate, 0)) {
        return principal * (monthly_rate * (pow((1 + monthly_rate), months)) /
                ((pow((1 + monthly_rate), months) - 1)));
    } else {
        return principal / months;
    }
}

double future_value(double pv, double rate, int periods) {
    return compound_interest_amount(pv, rate, periods);
}

double present_value(double fv, double rate, int periods) {
    enum errors_code valid_variables = check_variables(fv, rate, periods);

    if (valid_variables != NO_ERROR) return NAN;

    return fv / pow((1 + rate), periods);
}

int generate_amortization_schedule(double principal, double monthly_rate, int months, AmortizationTable* sched_table) {
    // Auxiliary variable for (1+i)^n
    double aux_1_p_rate_n;
    
    double monthly_due;  // Maybe const?
    double monthly_interest_pay;
    double monthly_principal_pay;

    double principal_due;
    double principal_due_yestermonth;

    // - Checks -
    // Checks if it is already filled
    if (sched_table->length != 0) return TABLE_ALREADY_ALLOCATED;

    int err = check_variables(principal, monthly_rate, months);
    if (err != NO_ERROR) return err;
    
    // - Memory allocation --
    sched_table->row_array = malloc(months * sizeof(AmortizationRow));
    sched_table->length = months;

    // - Values attributions -
    principal_due = principal;
    principal_due_yestermonth = principal_due;

    aux_1_p_rate_n = pow((1 + monthly_rate), months);    

    // Installment (fixed value)
    monthly_due = principal * ((monthly_rate * aux_1_p_rate_n) / (aux_1_p_rate_n - 1));

    for (int _m_i = 0; _m_i < months; _m_i++) {
        AmortizationRow* _sched_row = &(sched_table->row_array[_m_i]);

        monthly_interest_pay  = monthly_rate * principal_due_yestermonth;
        monthly_principal_pay = monthly_due - monthly_interest_pay;
        principal_due         = principal_due_yestermonth - monthly_principal_pay;
        
        principal_due_yestermonth = principal_due;
                
        // Insertion of information on schedule struct
        _sched_row->month             = _m_i + 1;
        _sched_row->installment       = monthly_due;
        _sched_row->interest          = monthly_interest_pay;
        _sched_row->remaining_balance = principal_due;
        _sched_row->amortization      = monthly_principal_pay;
    }

    return NO_ERROR;
}

AmortizationTable create_empty_schedule_table() {
    // TODO: Look into alternatives to this mess
    AmortizationTable sched_table;

    sched_table.length = 0;
    sched_table.row_array = NULL;

    return sched_table;
}

int load_csv_schedule(const char *file_path, const char *sep, AmortizationTable* sched_table) {
    // - Variables -
    char buf[4096];
    char format_string[128];
    int n_rows = 0;
    int i_row = 0;
    FILE *file;

    // - Preparations -
    if (sched_table->length != 0) return TABLE_ALREADY_ALLOCATED;

    sched_table->row_array = NULL;
    sched_table->length = 0;

    sprintf(format_string,
        "%%d%s%%lf%s%%lf%s%%lf%s%%lf",
        sep, sep, sep, sep);

    // - Allocation -
    n_rows = _count_file_lines(file_path);
    sched_table->row_array = malloc(n_rows * sizeof(AmortizationRow));
    sched_table->length = 0;

    // - File reading -
    file = fopen(file_path, "r");

    while (fgets(buf, sizeof(buf), file) != NULL) {
        int _n_read;

        int _month;
        double _installment;
        double _interest;
        double _amortization;
        double _remaining;       

        _n_read = sscanf(
            buf,
            format_string,
            &_month,
            &_installment,
            &_interest,
            &_amortization,
            &_remaining
        );

        // If couldn't read all 5 columns
        if (_n_read < 5) {
            free_schedule_table(sched_table);

            return TABLE_MISSING_COLUMNS;
        }

        sched_table->row_array[i_row].month             = _month;
        sched_table->row_array[i_row].interest          = _interest;
        sched_table->row_array[i_row].installment       = _installment;
        sched_table->row_array[i_row].amortization      = _amortization;
        sched_table->row_array[i_row].remaining_balance = _remaining;

        // Length is updated per successful line read
        sched_table->length = ++i_row;
    }

    fclose(file);
    
    return NO_ERROR;
}

void free_schedule_table(AmortizationTable *sched_table) {
    if (sched_table->row_array != NULL) {
        free(sched_table->row_array);
        sched_table->row_array = NULL;
    }
    sched_table->length = 0;
}
