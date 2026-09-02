/**
 * \file
 * 
 * \brief File containing functions for testing solver and format output
 */


#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"


/**
 * \defgroup SolverTests Solver tests functions
 * \brief Functions that are used to test functions that are intended to solve square equations
 */

 /**
 * \defgroup FormatTests Format tests functions
 * \brief Functions that are used to test format that's used in output of solutions of square equations
 */


/**
 * \ingroup SolverTests
 * 
 * \brief Function that does tests from file with given filename
 * 
 * If no file with given filename doesn't exist in current frame, function prints error message and return error code.
 * 
 * \param filename name of file that should contain tests for solver functions
 * 
 * \return Global constant code of success or error in function
 */
int test_solver_from_file(char filename[]) {
	int return_value = 0;
	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		printf(RED "Error: no such file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

	char * buf[MAX_BUF_LEN] = {0};
	for (int i = 0; i < MAX_BUF_LEN; ++i) {
		buf[i] = (char *) calloc(MAX_STR_LEN, sizeof(char));
	}

	int n_lines = copy_test_file_to_buf(buf, fp);

	fclose(fp);

	return_value = do_solver_tests(buf, n_lines);
	
	for (int i = 0; i < MAX_BUF_LEN; ++i) {
		free(buf[i]);
	}

	return return_value;
}


/**
 * \ingroup SolverTests
 * 
 * \brief Function that copies contents of file to buffer
 * 
 * Pointer to the file stream have be valid.
 * 
 * \param [in] buf buffer, array of strings in which contents of file are copied to
 * \param fp pointer to the file stream from which contents should be copied
 * 
 * \return Amount of copied from file lines
 */
int copy_test_file_to_buf(char *buf[MAX_BUF_LEN], FILE *fp) {

	assert(fp != NULL);

	char line[MAX_STR_LEN] = "";
	//todo read from buffer not from file - done

	int i = 0;
	while (fgets(line, MAX_STR_LEN, fp) != NULL) { // reading all lines from file
		strcpy(buf[i], line);
		++i;
	}
	return i; // returns amount of copied lines
}

/**
 * \ingroup SolverTests
 * 
 * \brief Function that does solver tests
 * 
 * \param buf buffer, array of strings containing tests for solver to do
 * \param n_lines amount of test lines in buffer
 * 
 * \return Global constant code of success or error in function
 */
int do_solver_tests(char *buf[MAX_BUF_LEN], int n_lines) {
	int n_tests = 0, n_wrong_tests = 0;


	for (int j = 0; j < n_lines; j++) {
		int err = 0;

		err = do_solver_test(buf[j]);
	
		if (err == GENERAL_ERROR) {
			return GENERAL_ERROR;
		}
		if (err == INCORRECT)
			n_wrong_tests++;
		n_tests++;
	} 
	printf("Wrong Answers: %d/%d\n", n_wrong_tests, n_tests);
	return CORRECT;
}


/**
 * \ingroup SolverTests
 * 
 * \brief Function that does one solver test from given test line
 * 
 * Test line should point to valid memory sector.
 * 
 * \param line string containing test information for doing test
 * 
 * \return Global constant code of success or error in function
 */
int do_solver_test(char line[]) {

	assert(line != NULL);

	square_equation test_true= {.a = NAN, .b = NAN, .c = NAN, .roots_code = 0, .x1 = NAN, .x2 = NAN};
	double x1 = NAN, x2 = NAN;
	int code = 0;

	if (do_reading_for_test_from_line(line, &test_true) == GENERAL_ERROR)
		return GENERAL_ERROR;

	code = square_equation_solve(test_true.a, test_true.b, test_true.c, &x1, &x2);

	return do_comparison_and_printing_of_wrong_tests(test_true, code, x1, x2);
}


/**
 * \ingroup SolverTests
 * 
 * \brief Function that reads test information from string to structure with given address
 * 
 * Structure pointer should point to valid memory sector.
 * 
 * \param [out] line string containing test information for doing test
 * \param [in, out] test pointer to square equation structure to which address information will be inserted
 * 
 * \return Global constant code of success or error in function
 */
int do_reading_for_test_from_line(char line[], square_equation *test) {

	assert(test != NULL);

	int err_flag = 0;
	if (sscanf(line, "%lg %lg %lg %d ", &test->a, &test->b, &test->c, &test->roots_code) != 4)
		err_flag = GENERAL_ERROR;

	switch(test->roots_code) {
	case ONE_ROOT_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg", &test->a, &test->b, &test->c, &test->roots_code, &test->x1) != 5)
			err_flag = GENERAL_ERROR;
		break;

	case TWO_ROOTS_CODE:
		if (sscanf(line, "%lg %lg %lg %d %lg %lg", &test->a, &test->b, &test->c, &test->roots_code, &test->x1, &test->x2) != 6)
			err_flag = GENERAL_ERROR;
		break;
	
	default:
		break;
	}

	if (err_flag == GENERAL_ERROR) {
		printf(RED "Error during reading" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}
	return CORRECT;
}


/**
 * \ingroup SolverTests
 * 
 * \brief Function that compares answer to correct one and prints them if test isn't passed
 * 
 * \param [out] test_true structure that contains information about corrct square equation roots
 * \param [out] code amount of roots code that solver functions output
 * \param [out] x1, x2 roots output from solver functions
 * 
 * \return Global constant code of success or error in function
 */
int do_comparison_and_printing_of_wrong_tests(square_equation test_true, int code, double x1, double x2) {
	if (code == test_true.roots_code) {
		if (test_true.roots_code == TWO_ROOTS_CODE)
			if ((is_doubles_equal(test_true.x1, x1) && is_doubles_equal(test_true.x2, x2)) || (is_doubles_equal(test_true.x1, x2) && is_doubles_equal(test_true.x2, x1)))
				return CORRECT; // correct
		if (test_true.roots_code == ONE_ROOT_CODE)
			if (is_doubles_equal(test_true.x1, x1))
				return CORRECT;
		if (test_true.roots_code == NO_ROOTS_CODE || test_true.roots_code == ANY_NUM_CODE)
			return CORRECT;
	}
	printf("Test FAILED: a = %lg, b = %lg, c = %lg\n"
	"Expected: x1: %10lf, x2: %10lf, roots code: %3d\n"
	"Got:      x1: %10lf, x2: %10lf, roots code: %3d\n\n", 
	test_true.a, test_true.b, test_true.c, 
	test_true.x1, test_true.x2, test_true.roots_code,
	x1, 			 x2, 		   code);
	return INCORRECT; //wrong
}


/**
 * \ingroup UserInterface
 * 
 * \brief Function that interacts with user to do format tests
 * 
 * Asks user to enter format tests file name, does tests and compares true values to returned from program
 * 
 * \return Global constant code of success or error in function
 */
int format_test_interactive_from_file() {
	char filename[MAX_STR_LEN] = "";
	int return_value = 0;

	printf("Enter test file name: ");

	if (scanf("%s", filename) != 1) {
		printf(RED "Error during filename reading" BASE_FMT ENDL);
		clear_buffer();
		return GENERAL_ERROR;
	}

	char * buf_program[MAX_BUF_LEN] = {0};
	char * buf_true[MAX_BUF_LEN] = {0};

	for (int i = 0; i < MAX_BUF_LEN; ++i) {
		buf_program[i] = (char *) calloc(MAX_STR_LEN, sizeof(char));
	}
	for (int i = 0; i < MAX_BUF_LEN; ++i) {
		buf_true[i] = (char *) calloc(MAX_STR_LEN, sizeof(char));
	}

	int test_err = read_format_test_from_file_to_buf(filename, buf_program, buf_true);

	if (test_err == GENERAL_ERROR) {
		printf(RED "Error while reading file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

	int buf_len = test_err;

	return_value = do_format_comparison_from_buf(buf_len, buf_program, buf_true);

	for (int i = 0; i < MAX_BUF_LEN; ++i) {
		free(buf_true[i]);
		free(buf_program[i]);
	}

	return return_value;
}



/**
 * \ingroup FormatTests
 * 
 * \brief Function that compares all test lines
 * 
 * Prints failed tests and summary.
 * 
 * \param buf_len amount of test lines in given buffer
 * \param buf_program buffer, array of strings that stores answers return by format output functions
 * \param buf_true buffer, array of strings that stores correct (read from file) answers for tests
 * 
 * \return Global constant code of success or error in function
 */
int do_format_comparison_from_buf(int buf_len, char *buf_program[MAX_BUF_LEN], char *buf_true[MAX_BUF_LEN]) {
	int n_tests = 0, n_failed_tests = 0, current_test_num = 0;

	
	
	for (int i = 0; i < buf_len; ++i) {
		char correct_ans[MAX_STR_LEN] = "", ans[MAX_STR_LEN] = "";

		custom_sgets(ans, buf_program[i]);
		custom_sgets(correct_ans, buf_true[i]);

		current_test_num++;

		n_tests++;
		if (!strcmp(ans, correct_ans));
		else {
			n_failed_tests++;
			printf("Wrong Answer on line %d.\nCorrect answer: %sYour answer: %s\n", current_test_num, correct_ans, ans);
		}
	}
	printf("Wrong Answers: %d/%d\n", n_failed_tests, n_tests);
	return CORRECT;
}


/**
 * \ingroup FormatTests
 * 
 * \brief Function that reads coefficients and correct answers to buffer and computes program answers and writes them to another buffer from file with given filename
 * 
 * \param filename string containing name of format test file
 * \param [out] buf_program buffer, array of strings that stores answers returned from program with read coefficients
 * \param [out] buf_true buffer, array of strings that stores correct answer with format
 * 
 * \return Global constant code of success or error in function
 */
int read_format_test_from_file_to_buf(char filename[MAX_STR_LEN], char *buf_program[MAX_BUF_LEN], char *buf_true[MAX_BUF_LEN]) {
	if (filename == NULL) {
		return GENERAL_ERROR;
	}

	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		return GENERAL_ERROR;
	}

	int i = 0;

	while (!feof(fp)) {
		double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;

		char correct_ans[MAX_STR_LEN] = "";
		char program_ans[MAX_STR_LEN] = "";

		fscanf(fp, "%lg %lg %lg ", &a, &b, &c); // temporary
		fgets(correct_ans, MAX_STR_LEN, fp);

		add_newline_symbol_to_str_end_if_theres_not(correct_ans);

		int n_sols = square_equation_solve(a, b, c, &x1, &x2);
		print_square_equation_sols_to_str(program_ans, x1, x2, n_sols);

		if (i >= MAX_BUF_LEN) {
			printf(RED "Error: file is too long" BASE_FMT ENDL);
			return GENERAL_ERROR;
		}

		strcat(buf_program[i], program_ans);
		strcat(buf_true[i], correct_ans);
		++i;
	}
	fclose(fp);
	return i;
}