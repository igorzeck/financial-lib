#include "finance.h"
#include <math.h>
#include <stdlib.h>

// TODO: Docstring
// TODO: Safety checks

// TODO: Do it with vargs
void check_variables(double principal, int periods) {
    // Made to avoid redundancies
    // TOOD: Fix this 'implicit' declaration
    if (principal < 0) exit(-1);
    if (periods <= 0) exit(-1);
}

double simple_interest(double principal, double rate, int periods) {
    check_variables(principal, periods);

    return principal * rate * periods;
}

double simple_interest_amount(double principal, double rate, int periods) {
    check_variables(principal, periods);

    return principal * (1 + rate * periods);
}

double compound_interest_amount(double principal, double rate, int periods) {
    check_variables(principal, periods);
    
    return principal * (pow((1 + rate), periods));
}

double compound_interest(double principal, double rate, int periods) {
    check_variables(principal, periods);
    
    return compound_interest_amount(principal, rate, periods) - principal;
}

double fixed_installment(double principal, double monthly_rate, int months) {
    // PMT = P * [ i * (1 + i)^n ] / [ (1 + i)^n - 1 ]
    // Here P = principal; i = monthly_rate; n = months
    
    if (fabs(monthly_rate) < 1e-9) {
        // TODO: Format this
        // TODO: Apply rule 3 of "Regras de Negócio"
        return principal * (monthly_rate * (pow((1 + monthly_rate), months)) / ((pow((1 + monthly_rate), months) - 1)));
    } else {
        return principal / months;
    }
}
