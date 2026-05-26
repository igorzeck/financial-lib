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

    // Choose between the interactive options
    choice_menu(principal, rate, periods);

    return 0;
}