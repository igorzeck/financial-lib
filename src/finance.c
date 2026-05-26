// #define for header added for redundancy sake.

// NOTE: Ideally table functions would be on their own.
//       But due to limitations of scope they are not.
#include "finance.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// - Helpers -

int is_close(double n1, double n2) {return fabs(n1 - n2) < THRESHOLD;}

int printf_currency(double value, const char* currency_str) {
    if (strlen(currency_str) > 3) {
        return CURRENCY_CODE_IS_TOO_LONG;
    }

    printf("%s %.2lf", currency_str, value);
    return NO_ERROR;
}

void fill_schedule_table(AmortizationTable* sched_table, AmortizationRow* row_array, int length) {
    sched_table->row_array = row_array;
    sched_table->length = length;
}

// Prints Schedule table header
void _print_schedule_header() {
    printf("| %-5s| %-19s| %-19s| %-19s| %-19s|\n",
    "Month","Installment","Interest","Amortization","Remaining");
    
    // Maybe putchar would be more standard?
    for (int i = 0; i < LINE_WIDTH; i++) printf("%c", '-');
    
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

void print_schedule_table(AmortizationTable sched_table) {    
    _print_schedule_header();
    for (int _m = 0; _m < sched_table.length; _m++) {
        _print_schedule_row(sched_table.row_array[_m]);
    }
    printf("Length: %d\n\n", sched_table.length);
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

    if (valid_variables != NO_ERROR) {
        fprintf(stderr, "Error: %d\n", valid_variables);
        return 0.0f;
    }

    return principal * rate * periods;
}

double simple_interest_amount(double principal, double rate, int periods) {
    enum errors_code valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != NO_ERROR) {
        fprintf(stderr, "Error: %d\n", valid_variables);
        return 0.0f;
    }

    return simple_interest(principal, rate, periods) + principal;
}

double compound_interest_amount(double principal, double rate, int periods) {
    enum errors_code valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != NO_ERROR) {
        fprintf(stderr, "Error: %d\n", valid_variables);
        return 0.0f;
    }

    return principal * (pow((1 + rate), periods));
}

double compound_interest(double principal, double rate, int periods) {
    // Error checked on 'compound_interest_amount'
    return compound_interest_amount(principal, rate, periods) - principal;
}

double fixed_installment(double principal, double monthly_rate, int months) {
    enum errors_code valid_variables = check_variables(principal, monthly_rate, months);
    
    if (valid_variables != NO_ERROR) {
        fprintf(stderr, "Error: %d\n", valid_variables);
        return 0;
    }

    if (!is_close(monthly_rate, 0)) {
        // TODO: Format this
        // TODO: Apply rule 3 of "Regras de Negócio"
        return principal * (monthly_rate * (pow((1 + monthly_rate), months)) / ((pow((1 + monthly_rate), months) - 1)));
    } else {
        return principal / months;
    }
}

double future_value(double pv, double rate, int periods) {
    // Error checked on 'compound_interest_amount'
    return compound_interest_amount(pv, rate, periods);
}

double present_value(double fv, double rate, int periods) {
    enum errors_code valid_variables = check_variables(fv, rate, periods);

    if (valid_variables != NO_ERROR) {
        fprintf(stderr, "Error: %d\n", valid_variables);
        return 0;
    }

    return fv / pow((1 + rate), periods);
}

int generate_amortization_schedule(double principal, double monthly_rate, int months, AmortizationTable *sched_table) {
    // auxiliary for (1+i)^n
    double aux_1_p_rate_n;
    
    double monthly_due;  // Maybe const?
    double monthly_interest_pay;
    double monthly_principal_pay;

    double principal_due;
    double principal_due_yestermonth;

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

    return 0;
}
