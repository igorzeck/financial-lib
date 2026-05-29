/*
File for automated tests.
*/

#include <stdio.h>
#include <stdlib.h>
#include "finance.h"
#include "utils.h"

int main()
{
    double principal = -1000;
    double rate = -0.02;
    int periods = 24;
    double cash_flow_arr[24] = {1, 0, 300, 400, 500, 600, 700, 15, 17, 19, 21, 2100, 2101, 2102, 2103, 26, 27, 28, 29, 30, 17, 19, 21, 23};

    AmortizationTable sched_table;
    // TODO: Function to initialize schedule tables
    // NOTE: There is a bug that make intallmemnet show as 0.0 when it should be nan for rate = 0
    //       This bug was not fixed to show the slice function on the tests.
    sched_table.row_array = NULL;
    sched_table.length = 0;

    // - Printing tests -
    printf("Compound interest: ");
    printf_currency(compound_interest(principal, rate, periods), "R$");
    printf("\n");
    printf("Compound interest amount: ");
    printf_currency(compound_interest_amount(principal, rate, periods), "R$");
    printf("\n");
    printf("Fixed installment: ");
    printf_currency(fixed_installment(principal, rate, periods), "R$");
    printf("\n");
    printf("Future value: ");
    printf_currency(future_value(principal, rate, periods), "R$");
    printf("\n");
    printf("Present value: ");
    printf_currency(present_value(future_value(principal, rate, periods),rate,periods), "R$");
    printf("\n");
    printf("Net Present Value:");
    double npv = net_present_value(principal, cash_flow_arr, rate, periods);
    printf_currency(npv, "R$");
    printf("\n");
    printf("Internal rate of return guess: \n");
    double irr = internal_rate_of_return(principal, cash_flow_arr, periods, -2, 2.1, 32);
    printf("   %.13lf", irr);
    printf("\n");
    printf("  Absolute error of: \n");
    double npv_abs_error = net_present_value(principal, cash_flow_arr, irr, periods);
    printf("   %.9lf\n", npv_abs_error);
    // exit(0);
    // - Auto tests -
    int _status; 

    // Tests up to 1e-14
    _status = financial_functions_comparison(1.0, THRESHOLD * 1e-5);
    
    if (_status != NO_ERROR) {
        printf("Unexpected error %d.\nEnding Test Routine.", _status);
        exit(_status);
    }

    // - Comparison for amortization table -
    amortization_comparison();

    printf("\nTests have been finished.\n");

    return 0;
}