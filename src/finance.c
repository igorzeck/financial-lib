#include "finance.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printf_currency(double value, char* currency_str) {
    if (strlen(currency_str) > 3) {
        fprintf(stderr, "printf_currency: currency code too long\n");
        return;
    }
    // TODO: Make this safe on 'currency_str'
    printf("%.2lf %s", value, currency_str);
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
