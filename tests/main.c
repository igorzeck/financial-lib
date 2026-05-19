#include <stdio.h>
#include "finance.h"
// TODO: Interactivity
// TODO: Judge

int main() {
    double principal = 1000.0;
    double rate = 0.05;
    int periods = 12;

    printf("Juros simples: %.2f\n", simple_interest(principal, rate, periods));
    printf("Montante simples: %.2f\n", simple_interest_amount(principal, rate, periods));
    printf("Montante composto: %.2f\n", compound_interest_amount(principal, rate, periods));
    printf("Juros compostos: %.2f\n", compound_interest(principal, rate, periods));

    return 0;
}