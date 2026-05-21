/*
File for interactive tests and exploration of the library features.
*/
#include <stdio.h>
#include "finance.h"
#include "interface.h"
// TODO: Interactivity
// TODO: Judge
// TODO: Link to images

int main()
{
    double principal = 0;
    double rate = 0;
    int periods = 0;

    // interactive_menu();

    input_menu(&principal, &rate, &periods);

    printf_currency(1000.00f, "R$111");

    printf("\n--- Values ---\n");
    printf("Simple interest: %.2f\n", simple_interest(principal, rate, periods));
    printf("Simple interest amount: %.2f\n", simple_interest_amount(principal, rate, periods));
    printf("Compound interest amount: %.2f\n", compound_interest_amount(principal, rate, periods));
    printf("Coumpound interest: %.2f\n", compound_interest(principal, rate, periods));

    return 0;
}