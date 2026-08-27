#ifndef TESTING_H

#define TESTING_H

int test_solver_from_file(char filename[]);
int copy_test_file_to_buf(char * buf[MAX_BUF_LEN], FILE * fp);
int do_solver_tests(char * buf[MAX_BUF_LEN], int n_lines);
int do_solver_test(char line[]);
int do_reading_for_test_from_line(char line[], square_equation * test);
int do_comparison_and_printing_of_wrong_tests(square_equation test_true, int code, double x1, double x2);
int format_test_interactive_from_file();
int do_format_test_from_file_to_buf(char filename[MAX_STR_LEN], char * buf_program[MAX_BUF_LEN], char * buf_true[MAX_BUF_LEN]);
int do_format_comparison_from_buf(int buf_len, char * buf_program[MAX_BUF_LEN], char * buf_true[MAX_BUF_LEN]);

#endif