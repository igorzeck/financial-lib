/*
File for interactive tests and exploration of the library features.
*/
#include <stdio.h>
#include <stdlib.h>
#include "finance.h"
#include "interface.h"

int main()
{
    double principal = 0;
    double rate = 0;
    int periods = 0;

    int option = 1;
    char user_input[3];
    int result;

    result = input_menu(&principal, &rate, &periods);

    if (result != NO_ERROR) {
        printf("Error %d\n", result);
        exit(result);
    }

    // TODO: pass this to the interface
    while(1) {
        int _result = 0;

        // Option for re-inputing data
        if (option == 1) {
            printf("What do you wish to calculate?\n"
                "  1. See all options\n"
                "  2. Simple interest\n"
                "  3. Simple interest amount\n"
                "  4. Compound interest amount\n"
                "  5. Compound interest\n"
                "  6. FV\n"
                "  7. PV\n"
                "  8. Generate amortization schedule\n"
                "  9. Re-input data\n"
                "  0. Exit\n"
            );
        }

        printf("Option: ");

        _result = get_number_input(user_input, &option, "%d");

        // Error validation
        if (_result == VALUE_NOT_FOUND) {
            printf("EOF input found.\n");
            return _result;
        }

        if (_result != NO_ERROR) {
            option = 1;
            printf("Invalid value!\n");
            continue;
        }

        // Exit
        if (!option) break;

        // Option resolver
        switch (option) {
            // Se all options
            case 1:
                continue;
            // Simple interest
            case 2:
                printf("Simple interest: ");
                printf_currency(simple_interest(principal, rate, periods), "R$");
                break;
            // Simple interest amount
            case 3:
                printf("Simple interest amount: ");
                printf_currency(simple_interest_amount(principal, rate, periods), "R$");
                break;
            // Compound interest amount
            case 4:
                printf("Compound interest amount: ");
                printf_currency(compound_interest_amount(principal, rate, periods), "R$");
                break;
            // Compound interest
            case 5:
                printf("Coumpound interest: ");
                printf_currency(compound_interest(principal, rate, periods), "R$");
                break;
            // FV
            case 6:
                printf("Future value: ");
                printf_currency(future_value(principal, rate, periods), "R$");
                break;
            // PV
            case 7:
                printf("Present value: ");
                printf_currency(present_value(future_value(principal, rate, periods), rate, periods), "R$");
                break;
            // Armotization schedule
            case 8:
                printf("\nFor a period of %d months:\n\n", periods);

                AmortizationTable __sched_table;

                __sched_table.row_array = malloc(periods * sizeof(AmortizationRow));
                __sched_table.length = periods;

                generate_amortization_schedule(principal, rate, periods, &__sched_table);

                print_schedule_table(__sched_table);

                // Dealocates memory
                free(__sched_table.row_array);
                break;
            // Re-input data
            case 9:
                input_menu(&principal, &rate, &periods);
                option = 1;
                break;
            default:
                printf("Invalid option!\n");
                option = 1;
        }

        printf("\n");
    }

    return 0;
}