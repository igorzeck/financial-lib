/*
File for automated tests.
*/

#include <stdio.h>
#include "finance.h"
// TODO: Interactivity
// TODO: Judge
// TODO: Link to images

int main()
{
    // double principal = 0;
    // double rate = 0;
    // int periods = 0;

    // printf("Juros simples: %.2f\n", simple_interest(principal, rate, periods));
    // printf("Montante simples: %.2f\n", simple_interest_amount(principal, rate, periods));
    // printf("Montante composto: %.2f\n", compound_interest_amount(principal, rate, periods));
    // printf("Juros compostos: %.2f\n", compound_interest(principal, rate, periods));

    // int term = 4; // YEARS
    double principal = 30000.0f;
    double monthly_rate = 0.0025f;
    int months = 48;
    AmortizationRow rows[48];

    generate_amortization_schedule(principal, monthly_rate, months, rows);

    AmortizationTable sched;

    // Unsafe! I should allocate memory here
    // TODO: Function to initalize them
    sched.row_array = rows;
    sched.length = 48;
    
    print_schedule_table(sched);

    head_schedule_table(sched);

    tail_schedule_table(sched);

    return 0;
}