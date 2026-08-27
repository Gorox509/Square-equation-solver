#ifndef UTIL_FUNCS_H

#define UTIL_FUNCS_H

int clear_buffer();
int is_str_all_space(char * str);
void print_ascii_cat();
int n_spaces_in_next_input();
int double_is_zero(double x);
int custom_isinf(double x);
int custom_isnan(double x);
void replace_newline_with_null_terminator(char str[MAX_STR_LEN]);
int is_doubles_equal(double x, double y);

#endif