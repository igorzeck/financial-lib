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
int is_close_threshold(double n1, double n2, double threshold) {
    return(fabs(n1 - n2) < threshold) ||
                   (isnan(n1) && isnan(n2));
}

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
    Amortization table comparisons.

    Tables are stores on tests/tables.

    This also works as a usage example of the `load_csv_schedule` function.
*/
void amortization_comparison() {
    char buf[1024];
    char curr_file_path[64];
    int curr_line = 1;

    FILE *f_in = fopen("tests/in.txt", "r");

    printf("\n- Amortization tables comparison -\n"
        "Using default Threshold: %.9lf\n\n",
        THRESHOLD
    );

    while (fgets(buf, sizeof(buf), f_in) != NULL) {
        int _err = NO_ERROR;

        int _months;
        double _loan;
        double _monthly_rate;

        AmortizationTable sched_table_in = create_empty_schedule_table();
        AmortizationTable sched_table_out = create_empty_schedule_table();

        printf("\nTable %d:\n\n", curr_line);

         // - Generate 'In' table -
        sscanf(
            buf,
            "%lf %lf %d",
            &_loan,
            &_monthly_rate,
            &_months
        );

        // Checks variables
        _err = check_variables(_loan, _monthly_rate, _months);
        
        if (_err != NO_ERROR) printf("  Got error: %d\n", _err);

        generate_amortization_schedule(
            _loan,
            _monthly_rate,
            _months,
            &sched_table_in
        );

        print_schedule_table(sched_table_in);
        
        // - Loads 'Out' table -
        snprintf(curr_file_path, 64, "tests/tables/tab%d.tsv", curr_line++);
    
        _err = load_csv_schedule(curr_file_path, "\t", &sched_table_out);

        // - Compare row-wise both tables -
        int max_size = sched_table_out.length > sched_table_in.length ? sched_table_in.length : sched_table_out.length;
        int line_diff = 0;

        for (int _i = 0; _i < max_size; _i++) {
            // Closeness and NANess
            int _cmp = is_close_threshold(sched_table_in.row_array[_i].month, sched_table_out.row_array[_i].month, THRESHOLD) &&
                        is_close_threshold(sched_table_in.row_array[_i].amortization, sched_table_out.row_array[_i].amortization, THRESHOLD) &&
                        is_close_threshold(sched_table_in.row_array[_i].installment, sched_table_out.row_array[_i].installment, THRESHOLD) &&
                        is_close_threshold(sched_table_in.row_array[_i].interest, sched_table_out.row_array[_i].interest, THRESHOLD) &&
                        is_close_threshold(sched_table_in.row_array[_i].remaining_balance, sched_table_out.row_array[_i].remaining_balance, THRESHOLD);
            
        if (_cmp != 1) {
                printf("   Difference on line: %d\n", _i + 1);
                line_diff++;
            }
        }

        if (_err == NO_ERROR) printf("  Total of different lines: %d\n", line_diff);

        free_schedule_table(&sched_table_in);
        free_schedule_table(&sched_table_out);
    }
}

#endif