#ifndef FINANCE_H
#define FINANCE_H
/*
File with definition of functions for calculation financial formulas.

On error, those with int return the error code.
While, those of other types may print to stderr and return 0.0f.

Before calling any function, one should check the variables.
*/

#include <sys/types.h>
#define LINE_WIDTH 92
#define THRESHOLD 1e-9  // For now, immutable

// -- Error codes --

/*
ERRORS enum type

NOTE: Some liberties were taken to emulate a binary mask on a enum.
*/
enum errors_code{
    // Value errors
    NO_ERROR=0,
    NEG_PRINCIPAL=2,
    NEG_PERIODS=4, // Also thrown if periods = zero
    NEG_PRINCIPAL_PERIODS=6,
    NEG_RATE=8,
    NEG_PRINCIPAL_RATE=10,
    NEG_PERIODS_RATE=12,
    NEG_PRINCIPAL_PERIODS_RATE=14,
    VALUE_NOT_FOUND=64,
    VALUE_IS_INVALID=65,
    VALUE_TYPE_NOT_FOUND=66,
    CURRENCY_CODE_IS_TOO_LONG=128,
    TABLE_ALREADY_ALLOCATED=256, // Remove this error?
    TABLE_MISSING_COLUMNS=257,
    FILE_NOT_OPENED=512,
    FILE_NOT_CREATED=513,
};

enum table_type{
    SCHEDULE_TABLE,
    SCHEDULE_SLICE,
};

// -- Structs --
// Structs declared here are meant to be visible for every caller

typedef struct {
    unsigned int month;
    double installment;
    double interest;
    double amortization;
    double remaining_balance;
} AmortizationRow;

// Table of AmortizationRows
typedef struct {
    unsigned int length;
    enum table_type type;
    AmortizationRow *row_array;
} AmortizationTable;

// -- Functions --
// Functions declared here meant to be visible for every caller

// - Helpers -

/*
    Float comparison with THRESHOLD macro.

    Return: 1 if |n1| - |n2| < Threshold.
*/
int is_close(double n1, double n2);

/*
currency_str: should be a NULL-TERMINATED currency stirng e.g. "USD" or "R$" and
              maximum length should be 3.
*/
int printf_currency(double value, const char* currency_str);

void _print_schedule_row(AmortizationRow sched_row);
void print_schedule_table(AmortizationTable sched_table);
void head_schedule_table(AmortizationTable sched_table);
void tail_schedule_table(AmortizationTable sched_table);

/*
Check common asked variables

Return: 0 if Ok, else error. 

Codes follow a additive pattern (ABC) -> A for Rate; B for periods; C for principal.

NOTE: Ideally, use before calling the variables.
*/
enum errors_code check_variables(double principal, double rate, int periods);

// - Finance related -

/* 
Simple interest

Return: principal * rate * periods
*/
double simple_interest(
    double principal, 
    double rate, 
    int periods
);

/*
Simple interest amount

Return: simple_interest() + principal
*/
double simple_interest_amount(
    double principal, 
    double rate, 
    int periods
);

/*
Compound interest amount

Return: principal * (1 + rate) ^ periods
*/
double compound_interest_amount(
    double principal, 
    double rate, 
    int periods
);

/*
Compound interest

Return: compound_interest_amount() - principal
*/
double compound_interest(
    double principal,
    double rate, 
    int periods
);

/*
Fixed installment

Return: principal * [ rate * (1 + rate)^n ] / [ (1 + rate)^n - 1 ]
*/
double fixed_installment(
    double principal, 
    double monthly_rate, 
    int months
);

/*
Future value (FV)

pv: Present value

Acts as a entry point for `compound_interest_amount`

Return: compound_interest_amount(pv, rate, periods)
*/
double future_value(
    double pv,
    double rate,
    int periods
);

/*
Present value (PV)

fv: future value

Return: fv / ((1 + rate) ^ periods)
*/
double present_value(
    double fv,
    double rate,
    int periods
);

/*
    Calculate the Net Present Value (NPV).

    cash_flow_arr contains N = periods cash_flow values (positive/negative).

    Return: sum((cash_flow_arr[i])/(1 - discount_rate)) - capital, for 0 < i < periods.
*/
double net_present_value(
    double capital,
    const double* cash_flow_arr,
    double discount_rate,
    int periods
);

/*
    Guesses best internal rate of return.

    Tries to split max_iterations times to find root for the `net_present_value`.

    As solving IRR "correctly" would involve solving a arbitrary polynomial equation,
    it was decided that a guess function would work best.

    The function does a binary search between start and finish, however, it does in such a way that it may get
    stuck on local minima.

    WARNING: The mean of start, end shouldn't equal zero. Otherwise, it might return NAN.
*/
double internal_rate_of_return(
    double capital,
    const double* cash_flow_arr,
    int periods,
    double start,
    double finish,
    int max_iterations
);

/*
Amortization schedule generator

Monthly amortized schedule uses the following formulae:

Total monthly payment (`monthly_due`):

`loan_amount * (monthly_interest_rate * (1 + monthly_interest_rate) ^ number_payments) / ((1 + monthly_interest_rate) ^ number_payments - 1))`

Pay to interest (`monthly_interest_pay`):

`monthly_rate * principal_due_yestermonth`

Pay to principal (`monthly_principal_pay`):

 `monthly_due - monthly_interest_pay`

Principal balance (`principal_due`):

`principal_due_yestermonth - monthly_principal_pay`

This function allocate the necessary memory. If the called
pass a filled table will return `TABLE_ALREADY_ALLOCATED` error.

If you wish to use the same struct is advised that you free it before with
`free_schedule_table`.
*/
int generate_amortization_schedule(
    double principal,
    double monthly_rate,
    int months,
    AmortizationTable* sched_table
);

/*
    Creates empty AmortizationTable object.
    Safer than generating one by hand.
*/
AmortizationTable create_empty_schedule_table();

/*
    Slices and return new object with sliced Amortization Table.

    The slice is by reference, therefore, data is not copied.

    Note that start and steps are UINT not INT.

    Returns: Amortization Table with sliced data. The slice is referencial.
*/
AmortizationTable slice_amortization_table(
    const AmortizationTable* sched_origin,
    unsigned int start,
    unsigned int steps
);

/*
    Loads table from CSV like file into sched_table.

    Needs to pass path and a field separator parameter.
*/
int load_schedule(
    const char* file_path,
    const char* sep,
    AmortizationTable* sched_table
);

/*
    Saves table to CSV like file.

    Needs to pass path and a field separator parameter.
*/
int save_schedule(
    AmortizationTable sched_table,
    const char* file_path,
    const char* sep
);

/*
    Frees memory allocated by schedule_table. Should be called after
    `generate_amortization_schedule`.
*/
void free_schedule_table(AmortizationTable* sched_table);

#endif