// Helper utilities for testing.
#ifndef UTILS_H
#define UTILS_H

#include "finance.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

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
int printf_comparison(const char* comparison_str, double calc_value, double exp_value, double threshold) {
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
    Function to compare financial function output.

    Iterates through threshold to test when failure occurs.
*/
int financial_functions_comparison(double threshold_steps, double max_threshold_steps) {
    // - Auto tests -
    double principal = 0.0f;
    double rate = 0.0f;
    int periods = 0;
    char buffer_in[1024];
    char buffer_out[1024];

    int counter = 1;

    // Threshold goes from "threshold_steps" to "max_threshold_steps"
    // At each step compares values in in.txt to values in out.txt

    int threshold_counter = 0;
    int first_fail_count = 0;
    int last_perfect_count = 0;

    do {
        printf("\nThreshold set to: 1-e%d\n", threshold_counter++);

        FILE *file_in = fopen("tests/in.txt", "r");
        FILE *file_out = fopen("tests/out.txt", "r");
        int line_counter = 0;
        double* cash_flow_arr = NULL;
        int max_cash_arr_size = 0;

        if (file_in == NULL || file_out == NULL) {
            fprintf(stderr, "Couldn't open test files!\n");
            return FILE_NOT_OPENED;
        }

        while (fgets(buffer_in, sizeof(buffer_in), file_in) != NULL &&
            fgets(buffer_out, sizeof(buffer_out), file_out) != NULL) {
            // - Variables -
            int _err;
            int _chars_read;
            
            printf("Comparison %d (line %d):\n", counter++, ++line_counter);

            // Fixed size variables
            sscanf(
                buffer_in,
                "%lf %lf %d%n",
                &principal,
                &rate,
                &periods,
                &_chars_read
            );

            // Checks variables
            _err = check_variables(principal, rate, periods);
            
            if (_err != NO_ERROR) printf("  Got error: %d\n", _err);

            // Dynamically allocates memory for double array
            if (periods >= 0) {
                // Reallocates only when cash_flow_arr size increases.
                if (periods > max_cash_arr_size) {
                    cash_flow_arr = realloc(cash_flow_arr, periods * sizeof(double));
                    max_cash_arr_size = periods;
                }
            }

            // Fills cash flow array
            char *ptr = buffer_in + _chars_read;
            char *end;
            int i = 0;

            while (*ptr != '\0') {
                double value = strtod(ptr, &end);

                if (ptr == end) break;

                cash_flow_arr[i++] = value;
                
                ptr = end;
            }

            // Attributions
            double _simple_interest          = simple_interest(principal, rate, periods);
            double _simple_interest_amount   = simple_interest_amount(principal, rate, periods);
            double _compound_interest_amount = compound_interest_amount(principal, rate, periods);
            double _compound_interest        = compound_interest(principal, rate, periods);
            double _fixed_installment        = fixed_installment(principal, rate, periods);
            double _future_value             = future_value(principal, rate, periods);
            double _present_value            = present_value(future_value(principal, rate, periods), rate, periods);
            double _net_present_value        = net_present_value(principal, cash_flow_arr,rate, periods);
            double _internal_rate_of_return  = internal_rate_of_return(principal, cash_flow_arr, periods, -2, 2.1, 24);

            // Expected values
            double _exp_simple_interest;
            double _exp_simple_interest_amount;
            double _exp_compound_interest_amount;
            double _exp_compound_interest;
            double _exp_fixed_installment;
            double _exp_future_value;
            double _exp_present_value;
            double _exp_net_present_value;
            double _exp_internal_rate_of_return;
            
            _err = sscanf(
                buffer_out,
                "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
                &_exp_simple_interest,
                &_exp_simple_interest_amount,
                &_exp_compound_interest_amount,
                &_exp_compound_interest,
                &_exp_fixed_installment,
                &_exp_future_value,
                &_exp_present_value,
                &_exp_net_present_value,
                &_exp_internal_rate_of_return
            );

            // - Comparisons -
            int _status = 0;

            _status += printf_comparison("Simple interest", _simple_interest, _exp_simple_interest, threshold_steps);
            _status += printf_comparison("Simple interest amount", _simple_interest_amount, _exp_simple_interest_amount, threshold_steps);
            _status += printf_comparison("Compound interest amount", _compound_interest_amount, _exp_compound_interest_amount, threshold_steps);
            _status += printf_comparison("Compound interest", _compound_interest, _exp_compound_interest, threshold_steps);
            _status += printf_comparison("Fixed installment", _fixed_installment, _exp_fixed_installment, threshold_steps);
            _status += printf_comparison("Future value", _future_value, _exp_future_value, threshold_steps);
            _status += printf_comparison("Present value", _present_value, _exp_present_value, threshold_steps);
            _status += printf_comparison("Net present value", _net_present_value, _exp_net_present_value, threshold_steps);
            _status += printf_comparison("Internal rate of return", _internal_rate_of_return, _exp_internal_rate_of_return, threshold_steps);

            if (_status) {
                printf("  Failed %d times with threshold of: %.*lf\n",
                    _status,
                    threshold_counter - 1,
                    threshold_steps
                );

                if (first_fail_count == 0) first_fail_count = threshold_counter;
            } else {
                last_perfect_count = threshold_counter;
            }
        }
        threshold_steps /= 10;
        
        fclose(file_in);
        fclose(file_out);

        free(cash_flow_arr);

    } while(threshold_steps > max_threshold_steps);

    // Report:
    printf("\nReport:\n");
    printf("First fail at 1-e%d\n", first_fail_count);
    printf("Last perfect iteration at 1-e%d\n", last_perfect_count);
    printf(
        "Note: 'nan' aren't counted as a fail.\n"
        "Note: 'internal_rate_of_return' interval used was -2 to 2.1, with 32 iterations.\n"
    );

    return NO_ERROR;
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
    
        _err = load_schedule(curr_file_path, "\t", &sched_table_out);

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
                AmortizationTable _sliced_table;

                _sliced_table = create_empty_schedule_table();

                // Instead of printing the whole row, here prints a slice
                // The row printing function is internal, therefore, shouldn't be used here
                printf("GENERATED:\n");
                _sliced_table = slice_amortization_table(&sched_table_in, _i, 1);
                print_schedule_table(_sliced_table);
                printf("EXPECTED:\n");
                _sliced_table = slice_amortization_table(&sched_table_out, _i, 1);
                print_schedule_table(_sliced_table);

                line_diff++;
            }
        }

        if (_err == NO_ERROR) printf("  Total of different lines: %d\n", line_diff);

        free_schedule_table(&sched_table_in);
        free_schedule_table(&sched_table_out);
    }
}

#endif