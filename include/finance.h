#ifndef FINANCE_H
#define FINANCE_H
// TODO: Make validations for upper limit for values
// TODO: Table to their own file
// TODO: IRR and NPV

#define LINE_WIDTH 92
#define THRESHOLD 1e-9  // For now, immutable

// -- Error codes --

/*
ERRORS enum type

NOTE: Some liberties were taken to account for additive error code.
*/
enum errors_code{
    // Value errors
    NO_ERROR=0,
    NEG_PRINCIPAL=1,
    NEG_PERIODS=10,
    NEG_PRINCIPAL_PERIODS=11,
    NEG_RATE=100,
    NEG_PRINCIPAL_RATE=101,
    NEG_PERIODS_RATE=110,
    NEG_PRINCIPAL_PERIODS_RATE=111,
};

// -- Structs --
// Structs declared here are meant to be visible for every calller

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
// Functions declared here meant to be visibile for every caller

// - Helpers -

int is_close(double n1, double n2);

/*
currency_str: should be a NULL-TERMINATED currency stirng e.g. "USD" or "R$" and
              maximum length should be 3.
*/
void printf_currency(double value, char* currency_str);

void fill_schedule_table(AmortizationTable* table, AmortizationRow* row_array, int length);

void print_schedule_row(AmortizationRow sched_row);
void print_schedule_table(AmortizationTable sched_table);
void head_schedule_table(AmortizationTable sched_table);
void tail_schedule_table(AmortizationTable sched_table);

// TODO: Maybe an aggregator between 3 functions?

/*
Check common asked variables

Returns: 0 if Ok, else error. 

Codes follow a additive pattern (ABC) -> A for Rate; B for periods; C for principal
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

// PMT = P * [ i * (1 + i)^n ] / [ (1 + i)^n - 1 ]
// Here P = principal; i = monthly_rate; n = months
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

Return:
*/
double present_value(
    double fv,
    double rate,
    int periods
);

/*
Armotization schedule generator

This function expects an already allocated schedule "AmortizationRow" struct.

Monthly armotized schedule uses the following formulae:

Total monthly payment (`monthly_due`):

`loan_amount * (montlhy_interest_rate * (1 + montlhy_interest_rate) ^ number_payments) / ((1 + montlhy_interest_rate) ^ number_payments - 1))`

Pay to interest (`monthly_interest_pay`):

`monthly_rate * principal_due_yestermonth`

Pay to principal (`monthly_principal_pay`):

 `monthly_due - monthly_interest_pay`

Princpal balance (`principal_due`):

`principal_due_yestermonth - monthly_principal_pay`
*/
int generate_amortization_schedule(
    double principal,
    double monthly_rate,
    int months,
    AmortizationTable* sched_table
);

#endif