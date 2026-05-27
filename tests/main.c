/*
File for automated tests.
*/

#include <stdio.h>
#include <stdlib.h>
#include "finance.h"
#include "utils.h"

int main()
{

    // - Printing tests -

    double principal = 1000;
    double rate = 0;
    int periods = 12;

    AmortizationTable sched_table;
    // TODO: Function to initialize schedule tables
    sched_table.row_array = NULL;
    sched_table.length = 0;

    printf("Compound interest: ");
    printf_currency(compound_interest(principal, rate, periods), "R$");
    printf("\n");
    printf("Compound interest amount: ");
    printf_currency(compound_interest_amount(principal, rate, periods), "R$");
    printf("\n");
    printf("Fixed installment: ");
    printf_currency(fixed_installment(principal, rate, periods), "R$");
    printf("\n");
    printf("Future value: ");
    printf_currency(future_value(principal, rate, periods), "R$");
    printf("\n");
    printf("Present value: ");
    printf_currency(present_value(future_value(principal, rate, periods),rate,periods), "R$");
    printf("\n");
    printf("Amortization table: \n");
    int _err = generate_amortization_schedule(principal, rate, periods, &sched_table);
    print_schedule_table(sched_table);
    printf("\n");

    // - Auto tests -
    char buffer_in[1024];
    char buffer_out[1024];

    int counter = 1;

    // Threshold goes from "threshold_steps" to "max_threshold_steps"
    // At each step compares values in in.txt to values in out.txt

    int threshold_counter = 0;
    double threshold_steps = 1.0f;
    double max_threshold_steps = 1e-18;
    
    int first_fail_count = 0;
    int last_perfect_count = 0;

    // TODO: Below to function
    do {
        printf("\nThreshold set to: 1-e%d\n", threshold_counter++);

        FILE *file_in = fopen("tests/in.txt", "r");
        FILE *file_out = fopen("tests/out.txt", "r");
        int line_counter = 0;

        if (file_in == NULL || file_out == NULL) {
            fprintf(stderr, "Couldn't open test files!\n");
            return -1;
        }

        while (fgets(buffer_in, sizeof(buffer_in), file_in) != NULL &&
            fgets(buffer_out, sizeof(buffer_out), file_out) != NULL) {
            // - Variables -
            int _err;
            
            printf("Comparison %d (line %d):\n", counter++, ++line_counter);

            sscanf(
                buffer_in,
                "%lf %lf %d",
                &principal,
                &rate,
                &periods
            );

            // Checks variables
            _err = check_variables(principal, rate, periods);
            
            if (_err != NO_ERROR) printf("  Got error: %d\n", _err);

            // Attributions
            double _simple_interest          = simple_interest(principal, rate, periods);
            double _simple_interest_amount   = simple_interest_amount(principal, rate, periods);
            double _compound_interest_amount = compound_interest_amount(principal, rate, periods);
            double _compound_interest        = compound_interest(principal, rate, periods);
            double _fixed_installment        = fixed_installment(principal, rate, periods);
            double _future_value             = future_value(principal, rate, periods);
            double _present_value            = present_value(future_value(principal, rate, periods), rate, periods);

            // Expected values
            double _exp_simple_interest;
            double _exp_simple_interest_amount;
            double _exp_compound_interest_amount;
            double _exp_compound_interest;
            double _exp_fixed_installment;
            double _exp_future_value;
            double _exp_present_value;
            
            _err = sscanf(
                buffer_out,
                "%lf %lf %lf %lf %lf %lf %lf",
                &_exp_simple_interest,
                &_exp_simple_interest_amount,
                &_exp_compound_interest_amount,
                &_exp_compound_interest,
                &_exp_fixed_installment,
                &_exp_future_value,
                &_exp_present_value
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

    } while(threshold_steps > max_threshold_steps);

    // Report:
    printf("\nReport:\n");
    printf("First fail at 1-e%d\n", first_fail_count);
    printf("Last perfect iteration at 1-e%d\n", last_perfect_count);
    printf(
        "Note: 'nan' aren't counted as a fail.\n"
        "Tests are finished now.\n"
    );

    // - Comparison for amortization table -
    amortization_comparison();

    printf("\n");

    return 0;
}