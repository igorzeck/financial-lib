#include <stdio.h>
#include "finance.h"
#include <stdlib.h>
#include <string.h>

int get_type_input(char* buffer, void* variable, const char* type, int buffer_size) {
    char* _status;
    int _result = -2;

    // This is made to limit input size (which scanf can't do)
    _status = fgets(buffer, buffer_size, stdin);

    if (_status == NULL) return VALUE_NOT_FOUND;

    if (strcmp(type, "lf") == 0) _result = sscanf(buffer, "%lf", (double*)variable);
    if (strcmp(type, "d") == 0) _result = sscanf(buffer, "%d", (int*)variable);
    
    if (_result == 0) return VALUE_IS_INVALID;

    if(_result == -2) return VALUE_TYPE_NOT_FOUND;

    return NO_ERROR;
}

int input_menu(double *ptr_principal, double *ptr_rate, int *ptr_periods)
{
    char user_input[32];

    int b_exit_input = 0;

    do
    {
        int _result = 0;
        do
        {
            printf("\n-- Data input --\n");
            while(1) {
                printf("Principal: ");
                
                _result = get_type_input(user_input, ptr_principal, "lf", 32);

                if (_result == NO_ERROR) break; else return _result;
            }

            while (1)
            {
                printf("Rate: ");
                
                _result = get_type_input(user_input, ptr_rate, "lf", 32);

                if (_result == NO_ERROR) break; else return _result;
            }

            while (1)
            {
                printf("Periods: ");
                
                _result = get_type_input(user_input, ptr_periods, "d", 32);

                if (_result == NO_ERROR) break; else return _result;
            }
            
        } while(check_variables(*ptr_principal, *ptr_rate, *ptr_periods));

        printf("\n--- Values inserted ---\n\n");
        printf("| Principal         | Rate              | Periods     |\n");
        for (int i = 0; i < 55; i++) putchar('=');
        printf("\n");
        printf("| %-18.2lf| %-18.2lf| %-12d|\n", *ptr_principal, *ptr_rate, *ptr_periods);
        for (int i = 0; i < 55; i++) putchar('-');
        printf("\n");
        printf("\nConfirm \n1. Yes \n0. No:\nOption: ");

        _result = get_type_input(user_input, &b_exit_input, "d", 32);

        if (_result != NO_ERROR) return _result;
    } while(!b_exit_input);

    return 0;
}

int choice_menu(double principal, double rate, int periods) {
    char user_input[256];
    int option = 1;

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
                "  9. NPV & IRR\n"
                "  10. Re-input data\n"
                "  0. Exit\n"
            );
        }

        printf("Option: ");

        _result = get_type_input(user_input, &option, "d", 32);

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
                printf("Compound interest: ");
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
            // Amortization schedule
            case 8:
                printf("\nFor a period of %d months:\n\n", periods);

                AmortizationTable _sched_table;

                generate_amortization_schedule(principal, rate, periods, &_sched_table);

                print_schedule_table(_sched_table);

                // Deallocates memory
                free_schedule_table(&_sched_table);
                break;
            // NPV and IRR
            case 9:
                printf("Cash-flow per period:\n");
                double* cash_flow_arr = NULL;
                double npv_value = 0.0f;
                double irr_value = 0.0f;

                // Malloc here is slower, but keeps memory usage tight
                cash_flow_arr = malloc(sizeof(double) * periods);

                for (int p = 0; p < periods; p++) {
                    printf("Period %d: ", p + 1);
                    double curr_value = 0.0f;

                    get_type_input(user_input, &curr_value, "lf", 256);

                    cash_flow_arr[p] = curr_value;
                }

                printf("NPV: ");
                npv_value = net_present_value(principal, cash_flow_arr, rate, periods);
                
                printf_currency(npv_value, "R$");

                printf("\n");
                
                irr_value = internal_rate_of_return(principal, cash_flow_arr, periods, -2.0, 2.1, 28);
                
                printf("IRR: %.3lf", irr_value);

                if (cash_flow_arr != NULL) free(cash_flow_arr);

                break;
            // Re-input data
            case 10:
                _result = input_menu(&principal, &rate, &periods);
                

                if (_result != NO_ERROR) {
                    printf("Error %d\n", _result);
                    exit(_result);
                }
                option = 1;
                break;
            default:
                printf("Invalid option!\n");
                option = 1;
        }

        printf("\n");
    }

    return NO_ERROR;
}
