/*
File for automated tests.
*/

#include <stdio.h>
#include <stdlib.h>
#include "finance.h"
#include "utils.h"

int main()
{
    // NOTE: There is a bug that make intallmemnet show as 0.0 when it should be nan for rate = 0
    //       This bug was not fixed to show the slice function on the tests.
    double principal = 100000;
    double rate = -1;
    int periods = 5;
    double cash_flow_arr[5] = {30000, 35000, 40000, 25000, 20000};

    AmortizationTable sched_table = create_empty_schedule_table();

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