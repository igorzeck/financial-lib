// Helper utilities for testing.
#ifndef UTILS_H
#define UTILS_H

#include "finance.h"
#include <stdio.h>
#include <math.h>

// Static so that it isn't included trhough multiple .c files
// In case it's needed elsewhere

/*
Variable threshold is_close
*/
int is_close_threshold(double n1, double n2, double threshold) {return fabs(n1 - n2) < threshold;}

/*
Prints comparison between two double (long-floats) values

Returns 0 if comparison is exact, else another value.
*/
static int printf_comparison(char* comparison_str, double calc_value, double exp_value, double threshold) {
    int b_comp;
    
    b_comp = is_close_threshold(calc_value, exp_value, threshold);

    printf(
        "   (%-3s) %s: %.17lf == %.17lf\n", 
        b_comp ? "OK": "NOK",
        comparison_str,
        calc_value,
        exp_value
    );

    return !b_comp;
}

#endif