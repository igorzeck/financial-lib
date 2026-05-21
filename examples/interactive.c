/*
File for interactive tests and exploration of the library features.
*/
#include <stdio.h>
#include <stdlib.h>
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

    int option = 1;
    char user_input[32];

    input_menu(&principal, &rate, &periods);

    // TODO: pass this to the interface
    while(1) {
        int _result = 0;

        // Option for re-inputing data
        if (option == 1) {
            printf("What do you wish to calculate?\n"
                "1. See all options\n"
                "2. Simple interest\n"
                "3. Simple interest amount\n"
                "4. Compound interest amount\n"
                "5. Compound interest\n"
                "6. Generate amortization schedule\n"
                "0. Exit\n"
            );
        }

        printf("Option: ");

        fgets(user_input, sizeof(user_input), stdin);
        _result = sscanf(user_input, "%1d", &option);

        // Error validaiton
        if (_result != 1) {
            option = 1;
            printf("Invalid value!\n");
            continue;
        }

        // Exit
        if (!option) break;

        // Option resolver
        switch (option) {
            case 1:
                continue;
            case 2:
                printf("Simple interest: ");
                printf_currency(simple_interest(principal, rate, periods), "R$");
                break;
            case 3:
                printf("Simple interest amount: ");
                printf_currency(simple_interest_amount(principal, rate, periods), "R$");
                break;
            case 4:
                printf("Compound interest amount: ");
                printf_currency(compound_interest_amount(principal, rate, periods), "R$");
                break;
            case 5:
                printf("Coumpound interest: ");
                printf_currency(compound_interest(principal, rate, periods), "R$");
                break;
            case 6:
                printf("\nFor a period of %d months:\n\n", periods);

                AmortizationRow* sched_arr;
                AmortizationTable __sched_table;

                sched_arr = malloc(periods * sizeof(AmortizationRow));

                generate_amortization_schedule(principal, rate, periods, sched_arr);

                __sched_table.row_array = sched_arr;
                __sched_table.length = periods;

                print_schedule_table(__sched_table);

                // Dealocates memory
                free(sched_arr);
                break;
            default:
                printf("Invalid option!\n");
                option = 1;
        }

        printf("\n");
    }

    return 0;
}