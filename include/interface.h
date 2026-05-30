// - Module for interactive interface -
// This interface is only for testing the library functionalities.
// NOTE: As this is an interactive part of the library it does print data.

// Simple implementation for a static menu
void clean_buffer();

int get_type_input(char* buffer, void* variable, const char* type, int buffer_size);
int input_menu(double *p_principal, double *p_rate, int *p_periods);
int choice_menu(double principal, double rate, int periods);
