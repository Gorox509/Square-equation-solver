#ifndef UTIL_FUNCS_H

#define UTIL_FUNCS_H

int clear_buffer();
int non_space_symbols_in_str(char * str);
void print_ascii_cat();
int n_spaces_in_next_input();
bool is_double_zero(double x);
bool custom_isinf(double x);
bool custom_isnan(double x);
bool custom_isfinite(double x);
void replace_newline_with_null_terminator(char str[MAX_STR_LEN]);
bool is_doubles_equal(double x, double y);
int custom_sgets(char str_dest[MAX_STR_LEN], char str_source[MAX_STR_LEN]);


#endif