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
    for (unsigned int _m = 0; _m < sched_table.length; _m++) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    _print_schedule_footer(sched_table.length);
}

void head_schedule_table(AmortizationTable sched_table) {
    _print_schedule_header();
    for (unsigned int _m = 0; (_m < 5) && (_m < sched_table.length); _m++) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}

void tail_schedule_table(AmortizationTable sched_table) {
    _print_schedule_header();
    for (unsigned int _m = sched_table.length - 1; _m >= (sched_table.length - 5); _m--) {
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

double net_present_value(double capital, const double *cash_flow_arr, double discount_rate, int periods) {
    // Note: The formula here used is somewhat flexible, so there is less need for variables checks.
    double total;

    if (discount_rate == -1) return NAN;

    total = -capital;

    for (int p = 0; p < periods; p++)
        total += cash_flow_arr[p]/pow(1 + discount_rate, p + 1);

    return total;
}

double internal_rate_of_return(double capital, const double *cash_flow_arr, int periods, double start, double end, int max_iterations) {
    // NOTE: Assumes end >= start
    // This method may get stuck on local minima
    // - Checks if there is convergence -
    // Convergence is checked using the direction of change, if it doesn't change, then it don't converges
    // This mean that values close to start and end might not be considered.
    // Alternatively would be to use the Intermediate Value Theorem for polynomials,
    // However, it might return no root in interval even if there is some for certain polynomials.
    // Note: There are some bug with some values not converging.

    double curr_rate;
    double rate_step;
    double npv;

    int curr_direction = 0; // > (-1) and < (1)
    int b_direction_changed = 0;

    curr_rate = (end + start) / 2;
    rate_step = (end - start) / 2;

    for (int i = 0; i < max_iterations; i++) {
        int old_curr_direction = curr_direction;

        npv = net_present_value(capital, cash_flow_arr, curr_rate, periods);

        rate_step /= 2;

        if (npv > 0) {
            curr_rate += capital > 0 ? rate_step : -rate_step;

            curr_direction = 1;
        } else {
            curr_rate += capital > 0 ? -rate_step : rate_step;

            curr_direction = -1;
        }

        if ((old_curr_direction != 0) && (old_curr_direction != curr_direction)) b_direction_changed = 1;
        
        if (is_close(npv, 0)) break;
    }

    if (!b_direction_changed) return NAN;
    
    return curr_rate;
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
    sched_table->type = SCHEDULE_TABLE;

    if (sched_table->row_array == NULL) return MALLOC_FAILED;

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
    return (AmortizationTable){.length = 0, .type = SCHEDULE_TABLE, .row_array = NULL};
}

AmortizationTable slice_amortization_table(const AmortizationTable* sched_origin, unsigned int start, unsigned int steps) {
    AmortizationTable sched_sliced;

    sched_sliced.type = SCHEDULE_SLICE;

    // If uint underflows here the code will handle it, so it's "safe" to be used here.    
    int start_length = (start < sched_origin->length) ? start : sched_origin->length - 1;
    int steps_length = ((start + steps) <= sched_origin->length) ? steps : (sched_origin->length - start_length);

    sched_sliced.length = steps_length;
    if (steps_length > 0) {
        sched_sliced.row_array = sched_origin->row_array + start_length;
    } else {
        sched_sliced.row_array = NULL;
    }

    return sched_sliced;
}

int load_schedule(const char *file_path, const char *sep, AmortizationTable* sched_table) {
    // - Variables -
    char buf[4096];
    char format_string[128];
    int n_rows = 0;
    int i_row = 0;
    FILE *fin;

    // - Preparations -
    if (sched_table->length != 0) return TABLE_ALREADY_ALLOCATED;

    sched_table->row_array = NULL;
    sched_table->length = 0;
    sched_table->type = SCHEDULE_TABLE;

    sprintf(format_string,
        "%%d%s%%lf%s%%lf%s%%lf%s%%lf",
        sep, sep, sep, sep);

    // - Allocation -
    n_rows = _count_file_lines(file_path);
    sched_table->row_array = malloc(n_rows * sizeof(AmortizationRow));
    sched_table->length = 0;

    if (sched_table->row_array == NULL) return MALLOC_FAILED;

    // - File reading -
    fin = fopen(file_path, "r");

    if (fin == NULL) return FILE_NOT_OPENED;

    while (fgets(buf, sizeof(buf), fin) != NULL) {
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

    fclose(fin);
    
    return NO_ERROR;
}

int save_schedule(AmortizationTable sched_table, const char *file_path, const char *sep) {
    FILE *fout;

    fout = fopen(file_path, "w");

    if (fout == NULL) return FILE_NOT_CREATED;

    char format_string[128];

    sprintf(format_string,
        "%%d%s%%lf%s%%lf%s%%lf%s%%lf\n",
        sep, sep, sep, sep);

    for (unsigned int l = 0; l < sched_table.length; l++) fprintf(
        fout,
        format_string,
        sched_table.row_array[l].month,
        sched_table.row_array[l].installment,
        sched_table.row_array[l].interest,
        sched_table.row_array[l].amortization,
        sched_table.row_array[l].remaining_balance);

    return NO_ERROR;
}

void free_schedule_table(AmortizationTable *sched_table) {
    if (sched_table->row_array != NULL) {
        
        if (sched_table->type == SCHEDULE_TABLE) free(sched_table->row_array);

        sched_table->row_array = NULL;
    }
    sched_table->length = 0;
}
