/**
 * \file
 * 
 * \brief Header file for util_funcs.cpp file
 * 
 * Contains prototypes of functions used
 */

#ifndef UTIL_FUNCS_H

#define UTIL_FUNCS_H

int clear_buffer();
bool is_str_all_space(char * str);
void print_ascii_cat();
bool is_next_input_all_space();
bool is_double_zero(double x);
bool custom_isinf(double x);
bool custom_isnan(double x);
bool custom_isfinite(double x);
void replace_newline_with_null_terminator(char str[MAX_STR_LEN]);
bool is_doubles_equal(double x, double y);
int custom_sgets(char str_dest[MAX_STR_LEN], char str_source[MAX_STR_LEN]);
void add_newline_symbol_to_str_end_if_theres_not(char str[MAX_STR_LEN]);

#endif