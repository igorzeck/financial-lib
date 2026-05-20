// -- Structs --
// Structs declared here are meant to be visible for every calller

typedef struct {
    int month;
    double installment;
    double interest;
    double amortization;
    double remaining_balance;
} AmortizationRow;

// -- Functions --
// Functions declared here meant to be visibile for every caller

// - Helpers -

/*
* currency_str: should be a NULL-TERMINATED currency stirng e.g. "USD" or "R$" and
*               maximum length should be 3.
*/
void printf_currency(double value, char* currency_str);

// TODO: Maybe an aggregator between 3 functions?


// Check common asked variables
//
// Returns: 0 if Ok, else error. 
//
// Codes follow a additive pattern (ABC) -> A for Rate; B for periods; C for principal
int check_variables(double principal, double rate, int periods);

// - Finance related -

/* Simple interest */
double simple_interest(
    double principal, 
    double rate, 
    int periods
);

double simple_interest_amount(
    double principal, 
    double rate, 
    int periods
);

double compound_interest_amount(
    double principal, 
    double rate, 
    int periods
);

double compound_interest(
    double principal,
    double rate, 
    int periods
);

// PMT = P * [ i * (1 + i)^n ] / [ (1 + i)^n - 1 ]
// Here P = principal; i = monthly_rate; n = months
double fixed_installment(
    double principal, 
    double monthly_rate, 
    int months
);

// Monthly principal due to amrtized loan:
// principal_payment:
// 
// total_monthly_payment - (outstanding_loan_balance * (monthly_rate / months))
// Total monthly payment:
//
// loan_amount * (montlhy_interest_rate * (1 + montlhy_interest_rate) ^ number_payments) / ((1 + montlhy_interest_rate) ^ number_payments - 1))
int generate_amortization_schedule(
    double principal,
    double monthly_rate,
    int months,
    AmortizationRow schedule[]
);
