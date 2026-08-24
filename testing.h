#ifndef TESTING_H

#define TESTING_H

int test_solver_from_file(char filename[]);
int do_solver_tests(FILE * fp);
int do_solver_test(char * line);
int is_doubles_equal(double x, double y);
int do_reading_for_test_from_line(char * line, square_equation * test);
int do_comparison_and_print_of_wrong_tests(square_equation test_true, int code, double x1, double x2);
int format_test_interactive_from_file();
int do_format_test_from_file(char * filename);
int do_format_comparison_from_file(FILE * fp);

#endif