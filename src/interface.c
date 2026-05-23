#include <stdio.h>
#include "finance.h"
#include <stdlib.h>
#include <string.h>

// TODO: As the interface is an interactive routine, it does print data

void clean_buffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

int get_number_input(char* buffer, void* variable, const char* type) {
    char* _status;
    int _result = -1;

    // This is made to limit input size (which scanf can't do)
    _status = fgets(buffer, sizeof(buffer), stdin);

    if (_status == NULL) return VALUE_NOT_FOUND;

    if (strcmp(type, "lf") == 0) _result = sscanf(buffer, "%lf", (double*)variable);
    if (strcmp(type, "d") == 0) _result = sscanf(buffer, "%d", (int*)variable);
    
    if (_result == 0) return VALUE_IS_INVALID;

    if(_result == -1) return VALUE_TYPE_NOT_FOUND;

    return NO_ERROR;
}

int input_menu(double *ptr_principal, double *ptr_rate, int *ptr_periods)
{
    char user_input[32];

    int b_exit_input = 1;

    // FIXME: Lots of repetition on code below

    do
    {
        int _result = 0;
        printf("\n-- Input de dados --\n");
        do
        {
            while(1) {
                printf("Principal: ");
                
                _result = get_number_input(user_input, ptr_principal, "lf");

                if (_result == NO_ERROR) break; else return _result;
            }

            while (1)
            {
                printf("Rate: ");
                
                _result = get_number_input(user_input, ptr_rate, "lf");

                if (_result == NO_ERROR) break; else return _result;
            }

            while (1)
            {
                printf("Periods: ");
                
                _result = get_number_input(user_input, ptr_periods, "d");

                if (_result == NO_ERROR) break; else return _result;
            }
            
        } while(check_variables(*ptr_principal, *ptr_rate, *ptr_periods));

        // TODO: Format this with F-string
        printf("\n--- Values inserted ---\n\n");
        printf("| Principal         | Rate              | Periods     |\n");
        printf("=======================================================\n");
        printf("| %-18.2lf| %-18.2lf| %-12d|\n", *ptr_principal, *ptr_rate, *ptr_periods);
        printf("-------------------------------------------------------\n");
        printf("\nConfirm \n0. Yes \n1. No:\nOption: ");

        _result = get_number_input(user_input, &b_exit_input, "d");

        if (_result != NO_ERROR) return _result;
    } while(b_exit_input == 1);

    return 0;
}
