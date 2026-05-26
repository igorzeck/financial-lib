#ifndef FINANCE_H
#define FINANCE_H
/*
File with definition of functions for calculation financial formulas.

On error, those with int return the error code.
While, those of other types may print to stderr and return 0.0f.

Before calling any function, one should check the variables.
*/
// TODO: Make validations for upper limit for values
// TODO: Table to their own file
// TODO: IRR and NPV

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
    NEG_PERIODS=4,
    NEG_PRINCIPAL_PERIODS=6,
    NEG_RATE=8,
    NEG_PRINCIPAL_RATE=10,
    NEG_PERIODS_RATE=12,
    NEG_PRINCIPAL_PERIODS_RATE=14,
    VALUE_NOT_FOUND=64,
    VALUE_IS_INVALID=65,
    VALUE_TYPE_NOT_FOUND=66,
    CURRENCY_CODE_IS_TOO_LONG=128,
};

// -- Structs --
// Structs declared here are meant to be visible for every caller

typedef struct {
    int month;
    double installment;
    double interest;
    double amortization;
    double remaining_balance;
} AmortizationRow;

// Table of AmortizationRows.
typedef struct {
    int length;
    AmortizationRow *row_array;
} AmortizationTable;

// -- Functions --
// Functions declared here meant to be visible for every caller

// - Helpers -

int is_close(double n1, double n2);

/*
currency_str: should be a NULL-TERMINATED currency stirng e.g. "USD" or "R$" and
              maximum length should be 3.
*/
int printf_currency(double value, const char* currency_str);

void fill_schedule_table(AmortizationTable* table, AmortizationRow* row_array, int length);

void _print_schedule_row(AmortizationRow sched_row);
void print_schedule_table(AmortizationTable sched_table);
void head_schedule_table(AmortizationTable sched_table);
void tail_schedule_table(AmortizationTable sched_table);

// TODO: Maybe an aggregator between 3 functions?

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
Amortization schedule generator

This function expects an already allocated schedule "AmortizationRow" struct.

Monthly amortized schedule uses the following formulae:

Total monthly payment (`monthly_due`):

`loan_amount * (monthly_interest_rate * (1 + monthly_interest_rate) ^ number_payments) / ((1 + monthly_interest_rate) ^ number_payments - 1))`

Pay to interest (`monthly_interest_pay`):

`monthly_rate * principal_due_yestermonth`

Pay to principal (`monthly_principal_pay`):

 `monthly_due - monthly_interest_pay`

Principal balance (`principal_due`):

`principal_due_yestermonth - monthly_principal_pay`
*/
int generate_amortization_schedule(
    double principal,
    double monthly_rate,
    int months,
    AmortizationTable* sched_table
);

#endif