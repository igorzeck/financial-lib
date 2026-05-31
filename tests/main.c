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
    // - Auto tests -
    int _status = NO_ERROR;
    
    // Tests from 1e0 to 1e-13
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