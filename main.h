#ifndef MAIN_H

#define MAIN_H

int square_equation_solve(double a, double b, double c, double * x1, double * x2);
int linear_equation_solve(double k, double b, double * x);
void print_sq_eq_sols_stdout(double x1, double x2, int n);
void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol);
int input_square_coefs(double * a, double * b, double * c);
int clear_buffer();
int sq_eq_interactive();
int is_str_all_space(char * str);
void print_ascii_cat();
int is_all_next_input_space();
int double_is_zero(double x);
int custom_isinf(double x);
int custom_isnan(double x);
void replace_newline_with_null_terminator(char str[MAX_STR_LEN]);

#endif 