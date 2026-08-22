#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"




int test_solver_from_file(char * filename);
int do_solver_test(char * line);
int is_doubles_equal(double x, double y);
int do_reading_for_test_from_line(char * line, double * a, double * b, double * c, int * code_true, double * x1_true, double * x2_true);
int do_comparison_and_print_of_wrong_tests(double a, double b, double c, int code_true, int code, double x1_true, double x2_true, double x1, double x2);

int test_solver_from_file(char * filename) {
	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		printf(RED "Error: no such file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

	char line[MAX_STR_SIZE] = "";
	int n = 0, m = 0;

	while (1) {
		int err = 0;

		fgets(line, MAX_STR_SIZE, fp);

		if (feof(fp)) {
			printf("Wrong Answers: %d/%d\n", m, n);
			return 0;
		}

		err = do_solver_test(line);
		
		if (err == GENERAL_ERROR)
			return GENERAL_ERROR;
		m += err;
		n++;
	}
}

// todo deal with magic const (output flag) - done
// todo split into functions - done
int do_solver_test(char * line) {
	double x1 = NAN, x2 = NAN, a = NAN, b = NAN, c = NAN, x1_true = NAN, x2_true = NAN;
	int code_true = 0, code = 0;

	if (do_reading_for_test_from_line(line, &a, &b, &c, &code_true, &x1_true, &x2_true))
		return GENERAL_ERROR;

	code = square_equation_solve(a, b, c, &x1, &x2);

	return do_comparison_and_print_of_wrong_tests(a, b, c, code_true, code, x1_true, x2_true, x1, x2);
}


int do_reading_for_test_from_line(char * line, double * a, double * b, double * c, int * code_true, double * x1_true, double * x2_true) {
	int err_flag = 0;
	if (sscanf(line, "%lg %lg %lg %d ", a, b, c, code_true) != 4)
		err_flag = GENERAL_ERROR;

	switch(*code_true) {
	case ONE_ROOT_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg", a, b, c, code_true, x1_true) != 5)
			err_flag = GENERAL_ERROR;
		break;

	case TWO_ROOTS_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg %lg", a, b, c, code_true, x1_true, x2_true) != 6)
			err_flag = GENERAL_ERROR;
		break;
	
	default:
		break;
	}

	if (err_flag == GENERAL_ERROR) {
		printf("Error during reading\n");
		return GENERAL_ERROR;
	}
	return 0;
}


int do_comparison_and_print_of_wrong_tests(double a, double b, double c, int code_true, int code, double x1_true, double x2_true, double x1, double x2) {
	if (code == code_true) {
		if (code_true == TWO_ROOTS_CODE)
			if ((is_doubles_equal(x1_true, x1) && is_doubles_equal(x2_true, x2)) || (is_doubles_equal(x1_true, x2) && is_doubles_equal(x2_true, x1)))
				return 0; // correct
		if (code_true == ONE_ROOT_CODE)
			if (is_doubles_equal(x1_true, x1))
				return 0;
		if (code_true == NO_ROOTS_CODE || code_true == ANY_NUM_CODE)
			return 0;
	}
	printf("Test FAILED: a = %lg, b = %lg, c = %lg\n"
	"Expected: x1: %10lg, x2: %10lg, roots code: %3d\n"
	"Got:      x1: %10lg, x2: %10lg, roots code: %3d\n", a, b, c, x1_true, x2_true, code_true, x1, x2, code);
	return 1; //wrong
}


int is_doubles_equal(double x, double y) {

	if (fabs(x - y) < epsilon)
		return 1;
	return 0;
}
