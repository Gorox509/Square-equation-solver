#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"
//------------------------------
#define ANY_NUM_CODE -3		//|
#define TWO_ROOTS_CODE -15	//|
#define ONE_ROOT_CODE 9		//|
#define NO_ROOTS_CODE 0		//|
//------------------------------

#define WRONG_COEFS_CODE -1

#define GENERAL_ERROR -13	//poison 


#define MAX_STR_SIZE 100


int test_solver_from_file(char * filename);
int do_solver_test(char * line);
int is_doubles_equal(double x, double y);

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


int do_solver_test(char * line) {
	double x1 = NAN, x2 = NAN, a = NAN, b = NAN, c = NAN, x1_true = NAN, x2_true = NAN;
	int output_flag = 1, code_true = 0, code = 0;

	if (sscanf(line, "%lg %lg %lg %d ", &a, &b, &c, &code_true) != 4)
		output_flag = GENERAL_ERROR;

	switch(code_true) {
	case ONE_ROOT_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg", &a, &b, &c, &code_true, &x1_true) != 5)
			output_flag = GENERAL_ERROR;
		break;

	case TWO_ROOTS_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg %lg", &a, &b, &c, &code_true, &x1_true, &x2_true) != 6)
			output_flag = GENERAL_ERROR;
		break;
	
	default:
		break;
	}

	if (output_flag == GENERAL_ERROR) {
		printf("Error during reading\n");
		return GENERAL_ERROR;
	}


	code = square_equation_solve(a, b, c, &x1, &x2);

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
	return output_flag; //wrong
}


int is_doubles_equal(double x, double y) {
	const double epsilon = 1e-6;

	if (fabs(x - y) < epsilon)
		return 1;
	return 0;
}
