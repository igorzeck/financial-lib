// Module for interactive interface
// This interface is only for tests purposes

// Simple implementation for a static menu
void clean_buffer();

/*
Gets user input from stdin

TODO: explain type
*/
int get_number_input(char* buffer, void* variable, const char* type);
int input_menu(double *p_principal, double *p_rate, int *p_periods);

