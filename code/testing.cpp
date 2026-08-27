#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"

int test_solver_from_file(char filename[]) {
	int return_value = 0;
	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		printf(RED "Error: no such file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}
	char buf[MAX_BUF_LEN][MAX_STR_LEN] = {0};
	int n_lines = copy_test_file_to_buf(buf, fp);
	fclose(fp);
	return_value = do_solver_tests(buf, n_lines);
	return return_value;
}

int copy_test_file_to_buf(char buf[MAX_BUF_LEN][MAX_STR_LEN], FILE * fp) {
	char line[MAX_STR_LEN] = "";
	//todo read from buffer not from file - done

	int i = 0;
	while (fgets(line, MAX_STR_LEN, fp) != NULL) { // reading all lines from file
		strcpy(buf[i], line);
		++i;
	}
	return i; // returns amount of copied lines
}


int do_solver_tests(char buf[MAX_BUF_LEN][MAX_STR_LEN], int n_lines) {
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


// todo deal with magic const (output flag) - done
// todo split into functions - done
int do_solver_test(char line[]) {
	square_equation test_true= {.a = NAN, .b = NAN, .c = NAN, .roots_code = 0, .x1 = NAN, .x2 = NAN};
	double x1 = NAN, x2 = NAN;
	int code = 0;

	if (do_reading_for_test_from_line(line, &test_true) == GENERAL_ERROR)
		return GENERAL_ERROR;

	code = square_equation_solve(test_true.a, test_true.b, test_true.c, &x1, &x2);

	return do_comparison_and_printing_of_wrong_tests(test_true, code, x1, x2);
}


int do_reading_for_test_from_line(char line[], square_equation * test) {
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


int format_test_interactive_from_file() {
	char filename[MAX_STR_LEN] = "";
	int return_value = 0;

	printf("Enter test file name: ");

	if (scanf("%s", filename) != 1) {
		printf(RED "Error during filename reading" BASE_FMT ENDL);
		clear_buffer();
		return GENERAL_ERROR;
	}

	char buf_program[MAX_BUF_LEN][MAX_STR_LEN] = {0};
	char buf_true[MAX_BUF_LEN][MAX_STR_LEN] = {0};

	int test_err = do_format_test_from_file_to_buf(filename, buf_program, buf_true);

	if (test_err == GENERAL_ERROR) {
		printf(RED "Error while reading file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

	int buf_len = test_err;

	return_value = do_format_comparison_from_buf(buf_len, buf_program, buf_true);

	return return_value;
}

int do_format_comparison_from_buf(int buf_len, char buf_program[MAX_BUF_LEN][MAX_STR_LEN], char buf_true[MAX_BUF_LEN][MAX_STR_LEN]) {
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
			printf("Wrong Answer on line %d.\nCorrect answer: %s\nYour answer: %s\n\n", current_test_num, correct_ans, ans);
		}
	}
	printf("Wrong Answers: %d/%d\n", n_failed_tests, n_tests);
	return CORRECT;
}


int do_format_test_from_file_to_buf(char filename[MAX_STR_LEN], char buf_program[MAX_BUF_LEN][MAX_STR_LEN], char buf_true[MAX_BUF_LEN][MAX_STR_LEN]) {
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