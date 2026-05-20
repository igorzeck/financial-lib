#include <stdio.h>

// TODO: Make selectable what is to be calculated

void clean_buffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

// int interactive_menu()
// {
//     return -1;
// }

int input_menu(double *p_principal, double *p_rate, int *p_periods)
{
    char arr_input[32];

    int b_exit_input = 1;

    // FIXME: Lots of repetiton on code below

    while (b_exit_input == 1)
    {
        int _result = 0;
        printf("\n-- Input de dados --\n");
        while (1)
        {
            printf("Principal: ");

            // This is made to limit input size (which scanf can't do)
            fgets(arr_input, sizeof(arr_input), stdin);

            _result = sscanf(arr_input, "%18lf", p_principal);

            if (*p_principal < 0)
            {
                printf("Warning: principal value shouldn't negative.\n");
                _result = -1;
            }

            if (_result != 1)
            {
                printf("Invalid value!\n");
                // clean_buffer();
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Rate: ");

            fgets(arr_input, sizeof(arr_input), stdin);

            _result = sscanf(arr_input, "%18lf", p_rate);

            if (*p_rate < 0)
            {
                printf("Warning: rate value shouldn't negative.\n");
                _result = -1;
            }

            if (_result != 1)
            {
                printf("Invalid value!\n");
                // clean_buffer();
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Periods: ");

            fgets(arr_input, sizeof(arr_input), stdin);

            _result = sscanf(arr_input, "%12d", p_periods);

            if (*p_periods <= 0)
            {
                printf("Warning: period should be greater than zero.\n");
                _result = -1;
            }

            if (_result != 1)
            {
                printf("Invalid value!\n");
                // clean_buffer();
            }
            else
            {
                break;
            }
        }

        // TODO: Fomat this with F-string
        printf("\n--- Values inserted ---\n\n");
        printf("| Principal         | Rate              | Periods     |\n");
        printf("=======================================================\n");
        printf("| %-18.2lf| %-18.2lf| %-12d|\n", *p_principal, *p_rate, *p_periods);
        printf("-------------------------------------------------------\n");
        printf("\nConfirm (0 - Yes 1 - No):\nOption: ");
        _result = scanf("%d", &b_exit_input);
    }

    return 0;
}