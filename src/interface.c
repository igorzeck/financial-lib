#include <stdio.h>
#include "finance.h"

// TODO: Make selectable what is to be calculated

void clean_buffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
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

                // This is made to limit input size (which scanf can't do)
                fgets(user_input, sizeof(user_input), stdin);

                _result = sscanf(user_input, "%18lf", ptr_principal);

                if (_result == 1) break; else printf("Invalid value!\n");
            }

            while (1)
            {
                printf("Rate: ");

                fgets(user_input, sizeof(user_input), stdin);

                _result = sscanf(user_input, "%18lf", ptr_rate);

                if (_result == 1) break; else printf("Invalid value!\n");
            }

            while (1)
            {
                printf("Periods: ");

                fgets(user_input, sizeof(user_input), stdin);

                _result = sscanf(user_input, "%12d", ptr_periods);

                if (_result == 1) break; else printf("Invalid value!\n");
            }
            
        } while(check_variables(*ptr_principal, *ptr_rate, *ptr_periods));

        // TODO: Format this with F-string
        printf("\n--- Values inserted ---\n\n");
        printf("| Principal         | Rate              | Periods     |\n");
        printf("=======================================================\n");
        printf("| %-18.2lf| %-18.2lf| %-12d|\n", *ptr_principal, *ptr_rate, *ptr_periods);
        printf("-------------------------------------------------------\n");
        printf("\nConfirm \n0. Yes \n1. No:\nOption: ");

        
        fgets(user_input, sizeof(user_input), stdin);

        _result = sscanf(user_input, "%d", &b_exit_input);
    } while(b_exit_input == 1);

    return 0;
}
