#include "finance.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Ideally should be on a MACRO for pre-compilation
const int LINE_WIDTH = 76;

void printf_currency(double value, char* currency_str) {
    if (strlen(currency_str) > 3) {
        fprintf(stderr, "printf_currency: currency code too long\n");
        return;
    }
    // TODO: Make this safe on 'currency_str'
    printf("%.2lf %s", value, currency_str);
}

void print_schedule_header() {
    printf("|%-5s|%-19s|%-8s|%-19s|%-19s|\n",
    "Month","Installment","Interest","Amortization","Remaining");
    
    // Maybe putchar would be more standard?

    for (int i = 0; i < LINE_WIDTH; i++) printf("%c", '-');
    
    printf("\n");
}

void print_schedule_row(AmortizationRow sched_row) {
    printf("|%-5d|%-19lf|%-8lf|%-19lf|%-19lf|\n", 
        sched_row.month,
        sched_row.installment,
        sched_row.interest,
        sched_row.amortization,
        sched_row.remaining_balance
    );
}

void print_schedule_table(AmortizationTable sched_table) {    
    print_schedule_header();
    for (int _m = 0; _m < sched_table.length; _m++) {
        print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}

void head_schedule_table(AmortizationTable sched_table) {
    print_schedule_header();
    for (int _m = 0; (_m < 5) && (_m < sched_table.length); _m++) {
        print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}

void tail_schedule_table(AmortizationTable sched_table) {
    print_schedule_header();
    for (int _m = sched_table.length - 1; (_m >= 0) && (_m >= (sched_table.length - 5)); _m--) {
        print_schedule_row(sched_table.row_array[_m]);
    }
    printf("\n");
}


int check_variables(double principal, double rate, int periods) {
    int error_code = 0;

    if (principal < 0) {
        printf("Error: principal can't be negative.\n");
        error_code += 1;
    }

    if (periods < 0) {
        printf("Error: period should be greater than zero.\n");
        error_code += 10;
    }

    if (rate < 0) {
        printf("Error: rate shouldn't be zero.\n");
        error_code += 100;
    }

    return error_code;
}

// - Financial functions --

double simple_interest(double principal, double rate, int periods) {
    int valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != 0) exit(valid_variables);

    return principal * rate * periods;
}

double simple_interest_amount(double principal, double rate, int periods) {
    int valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != 0) exit(valid_variables);

    return principal * (1 + rate * periods);
}

double compound_interest_amount(double principal, double rate, int periods) {
    int valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != 0) exit(valid_variables);

    return principal * (pow((1 + rate), periods));
}

double compound_interest(double principal, double rate, int periods) {
    int valid_variables = check_variables(principal, rate, periods);

    if (valid_variables != 0) exit(valid_variables);
    
    return compound_interest_amount(principal, rate, periods) - principal;
}

double fixed_installment(double principal, double monthly_rate, int months) {
    int valid_variables = check_variables(principal, monthly_rate, months);

    if (valid_variables != 0) exit(valid_variables);

    if (fabs(monthly_rate) < 1e-21) {
        // TODO: Format this
        // TODO: Apply rule 3 of "Regras de Negócio"
        return principal * (monthly_rate * (pow((1 + monthly_rate), months)) / ((pow((1 + monthly_rate), months) - 1)));
    } else {
        return principal / months;
    }
}

int generate_amortization_schedule(double principal, double monthly_rate, int months, AmortizationRow *schedule) {
    // (1+i)^n
    double aux_1_p_rate_n;
    
    double monthly_due;  // Maybe const?
    double monthly_interest_pay;
    double monthly_principal_pay;

    double principal_due;
    double principal_due_yestermonth;

    // - Values atributions -
    principal_due = principal;
    principal_due_yestermonth = principal_due;

    aux_1_p_rate_n = pow((1 + monthly_rate), months);    

    // Installment (fixed value)
    monthly_due = principal * ((monthly_rate * aux_1_p_rate_n) / (aux_1_p_rate_n - 1));

    for (int _m_i = 0; _m_i < months; _m_i++) {
        monthly_interest_pay  = monthly_rate * principal_due_yestermonth;
        monthly_principal_pay = monthly_due - monthly_interest_pay;
        principal_due         = principal_due_yestermonth - monthly_principal_pay;
        
        principal_due_yestermonth = principal_due;
                
        // Insertion of information on schedule struct
        schedule[_m_i].month             = _m_i + 1;
        schedule[_m_i].installment       = monthly_due;
        schedule[_m_i].interest          = monthly_rate;
        schedule[_m_i].remaining_balance = principal_due;
        schedule[_m_i].amortization      = principal;
    }

    return 0;
}
