// Helper utilities for testing.
#ifndef UTILS_H
#define UTILS_H

#include "finance.h"
#include <stdio.h>
#include <math.h>

// Static so that it isn't included through multiple .c files
// In case it's needed elsewhere

/*
Variable threshold is_close
*/
int is_close_threshold(double n1, double n2, double threshold) {return fabs(n1 - n2) < threshold;}

/*
Prints comparison between two double (long-floats) values

Returns 0 if comparison is exact, else another value.
*/
static int printf_comparison(const char* comparison_str, double calc_value, double exp_value, double threshold) {
    int b_comp;

    b_comp = is_close_threshold(calc_value, exp_value, threshold)
            ||
            (isnan(calc_value) && isnan(exp_value));

    printf(
        "   (%-3s) %s: %.17lf == %.17lf\n", 
        b_comp ? "OK": "NOK",
        comparison_str,
        calc_value,
        exp_value
    );

    return !b_comp;
}

/*
    Amortization table comparisons
*/
void amortization_comparison() {
    char buf[1024];
    char curr_file_path[64];
    int status = NO_ERROR;
    int curr_line = 1;

    FILE *f_in = fopen("tests/in.txt", "r");

    while (fgets(buf, sizeof(buf), f_in) != NULL) {
        printf("\nTable %d:\n\n", curr_line);

        // AmortizationTable sched_table_in = create_empty_schedule_table();
        AmortizationTable sched_table_out = create_empty_schedule_table();
        
        snprintf(curr_file_path, 64, "tests/tables/tab%d.tsv", curr_line++);
        
        status = load_csv_schedule(curr_file_path, "\t", &sched_table_out);

        // The comparison is made with two schedule_table objects
        // One of them is loaded from tests/tables folder
        // print_schedule_table(sched_table_in);
        print_schedule_table(sched_table_out);

        free_schedule_table(&sched_table_out);
    }
}

#endif