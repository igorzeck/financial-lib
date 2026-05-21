/*
File for automated tests.
*/

#include <stdio.h>
#include <stdlib.h>
#include "finance.h"
// TODO: Judge
// TODO: Link to images

int main()
{
    double principal = 1000;
    double rate = 0;
    int periods = 5;

    // printf("Juros simples: %.2f\n", simple_interest(principal, rate, periods));
    // printf("Montante simples: %.2f\n", simple_interest_amount(principal, rate, periods));
    // printf("Montante composto: %.2f\n", compound_interest_amount(principal, rate, periods));
    // printf("Juros compostos: %.2f\n", compound_interest(principal, rate, periods));
    printf("Fixed installment: ");
    printf_currency(fixed_installment(principal, rate, periods), "R$");
    printf("\n");
    printf("Future value: ");
    printf_currency(future_value(principal, rate, periods), "R$");
    printf("\n");
    printf("Present value: ");
    printf_currency(present_value(future_value(principal, rate, periods),rate,periods), "R$");
    printf("\n");

    // int term = 4; // YEARS
    // double principal = 30000.0f;
    // double monthly_rate = 0.0025f;
    // int months = 48;
    // AmortizationRow rows[48];
    // AmortizationTable sched;

    // fill_schedule_table(&sched, rows, months);

    // generate_amortization_schedule(principal, monthly_rate, months, &sched);

    // print_schedule_table(sched);

    // head_schedule_table(sched);

    // tail_schedule_table(sched);

    return 0;
}