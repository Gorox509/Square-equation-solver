#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>



const int MAX_STR_SIZE = 100;

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"
//-------------------------------------
const int ANY_NUM_CODE = -3;		//|
const int TWO_ROOTS_CODE = -15;	    //|
const int ONE_ROOT_CODE = 9;		//|
const int NO_ROOTS_CODE = 0;		//|
//-------------------------------------

const int WRONG_COEFS_CODE = -1;

const int GENERAL_ERROR = -13;	//poison 

// todo use consts instead of defines - done +-

const double epsilon = 1e-6;


int square_equation_solve(double a, double b, double c, double * x1, double * x2);

int linear_equation_solve(double a, double b, double * x);

void print_sq_eq_sols_stdout(double x1, double x2, int n);

void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol);

int input_square_coefs(double * a, double * b, double * c);

int clear_buffer();

int sq_eq_interactive();

int format_test_interactive_from_file();

int do_format_test_from_file(char * filename);

int is_str_all_space(char * str);

void print_ascii_cat();

int is_all_next_input_space();

int double_is_zero(double x);

int do_format_comparison_from_file(FILE * fp);


#include "testing.cpp"



// сделать тесты - в процессе, макросы - есть
// enum  это круто фр фр
int main() {
	int cmd = 0;

	printf("Possible commands:\n"
		   "\'q\': quit\n"
		   "\'s\': solve equation\n\'f\': test format from file with\n\'t\': test solver from file\nEnter command: ");

	while ((cmd = getchar()) != EOF) {
		if (isspace(cmd))
			continue;

		if (!is_all_next_input_space())
			cmd = 0;

		switch (cmd) {
		case 'q':
			return 0;

		case 'f':
			format_test_interactive_from_file();
			break;

		case 't': {
			char filename[MAX_STR_SIZE] = "";
			printf("Enter test file name: ");
			if (scanf("%s", filename) != 1) {
				printf(RED "Error during filename reading" BASE_FMT ENDL);
				break;
			}
			test_solver_from_file(filename);
			break;
		}

		case 's':
			sq_eq_interactive();
			break;

		case 'c':
			print_ascii_cat();
			break;

		default:
			printf(RED "Wrong input. Try again\n" BASE_FMT);
			break;
		}
		printf("Enter command: ");

	}
	return 0;
}


int square_equation_solve(double a, double b, double c, double * x1, double * x2)	{ // ax^2 + bx + c = 0
	
	if (!isfinite(a) || !isfinite(b) || !isfinite(c) || x1 == NULL || x2 == NULL)
		return -1;

	*x1 = NAN;
	*x2 = NAN;

	if (a < 0.) {
		a = fabs(a);
		b *= -1.;
		c *= -1;
	}

	if (double_is_zero(a))
		return linear_equation_solve(b, c, x1);

	double D = b * b - 4 * a * c;

	if (D < 0)
		return NO_ROOTS_CODE;

	else if (double_is_zero(D)) {
		*x1 = (0 - b) / (2 * a);
		return ONE_ROOT_CODE;
	}

	// todo (check inf + nan) - done; + implement isinf() + isnan() - я хз как
	else {
		*x1 = (0 - b - sqrt(D)) / (2 * a);
		*x2 = (0 - b + sqrt(D)) / (2 * a);
		return TWO_ROOTS_CODE;
	}
}

int linear_equation_solve(double k, double b, double * x) { // ax + b = 0
	//todo global const (epsilon) - done || func - done
	if (k < 0.) {
		k = fabs(k);
		b *= -1;
	}

	if (double_is_zero(k)) {
		if (double_is_zero(b)) {
			return ANY_NUM_CODE; // any number
		}
		return NO_ROOTS_CODE; // no solutions
	}

	*x = (0 - b) / k;
	return ONE_ROOT_CODE;
}


void print_sq_eq_sols_stdout(double x1, double x2, int n_sol) {
	print_sq_eq_sols_fp(stdout, x1, x2, n_sol);
}


void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol) { // todo zero ptr & other assertions - done;;;;
	if (fp == NULL) 
		exit(0);

	if (isinf(x1) || isinf(x2)) //can be NaN;
		exit(0);

	switch(n_sol) {
	case NO_ROOTS_CODE:
		fprintf(fp, YELLOW "No solutions" BASE_FMT ENDL);
		break;

	case ONE_ROOT_CODE:
		fprintf(fp, YELLOW "Solution: %lg" BASE_FMT ENDL, x1);
		break;

	case TWO_ROOTS_CODE:
		fprintf(fp, GREEN "Solutions: %lg %lg" BASE_FMT ENDL, x1, x2);
		break;

	case ANY_NUM_CODE:
		fprintf(fp, YELLOW "Any number" BASE_FMT ENDL);
		break;

	case WRONG_COEFS_CODE:
		fprintf(fp, RED "Error: invalid coefficients" BASE_FMT ENDL);
		break;

	default:
		fprintf(fp, RED "Error" BASE_FMT ENDL);
		break;
	}
}

int input_square_coefs(double * a, double * b, double * c) {

	printf("Enter coefs: ");

	for (int attempts = 10; attempts > -1; --attempts) {

		if (scanf("%lg %lg %lg", a, b, c) == 3) {
			char next[MAX_STR_SIZE] = "";
			fgets(next, MAX_STR_SIZE, stdin);
			if (strlen(next) == 0 || is_str_all_space(next))
				return 0;
		}

		if (attempts)
			printf(RED "Wrong input format. Try again: " BASE_FMT);

		clear_buffer();
	}
	printf(RED "Too many attempts.\n" BASE_FMT);
	return GENERAL_ERROR;
}


int clear_buffer() {

	int ch = 0, i = 0;

	while (ch != EOF && ch != '\n') {
		ch = getchar();
		++i;
	}
	return i;
}


int sq_eq_interactive() {
	double a = NAN, b = NAN, c = NAN;
	double x1 = NAN, x2 = NAN;
	int n_sol = 0, input_err = 0;

	input_err = input_square_coefs(&a, &b, &c);

	if (input_err == GENERAL_ERROR) {
		return GENERAL_ERROR;
	}

	n_sol = square_equation_solve(a, b, c, &x1, &x2);

	print_sq_eq_sols_stdout(x1, x2, n_sol);

	return 0;
}


int format_test_interactive_from_file() {
	char filename[MAX_STR_SIZE] = "";

	printf("Enter test file name: ");

	if (scanf("%s", filename) != 1) {
		printf(RED "Error during filename reading" BASE_FMT ENDL);
		return -1;
	}

	int test_err = do_format_test_from_file(filename);

	if (test_err == GENERAL_ERROR) {
		printf(RED "Error: no such file" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

	FILE * fp = fopen(".temp", "r");

	return do_format_comparison_from_file(fp);
}

int do_format_comparison_from_file(FILE * fp) {
	int n_tests = 0, n_failed_tests = 0, i = 0; // todo rename - done
	while (1) {
		i++;
		char correct_ans[MAX_STR_SIZE] = "", ans[MAX_STR_SIZE] = "";

		if (fgets(correct_ans, MAX_STR_SIZE, fp) == NULL || fgets(ans, MAX_STR_SIZE, fp) == NULL) {
			printf("Wrong Answers: %d/%d\n", n_failed_tests, n_tests);
			fclose(fp);
			remove(".temp");
			return 0;
		}
		n_tests++;
		if (!strcmp(ans, correct_ans));
		else {
			n_failed_tests++;
			printf("Wrong Answer №%d.\nCorrect answer: %sYour answer: %s\n", i, correct_ans, ans);
		}
	}
}


int do_format_test_from_file(char filename[]) {

	if (filename == NULL)
		return GENERAL_ERROR;

	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		return GENERAL_ERROR;
	}

	FILE * tempp = fopen(".temp", "w+");
	while (1) {
		double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;

		char correct_ans[MAX_STR_SIZE] = "";

		if (feof(fp)) {
			fclose(tempp);
			fclose(fp);
			return 0;
		}

		fscanf(fp, "%lg %lg %lg ", &a, &b, &c);
		fgets(correct_ans, MAX_STR_SIZE, fp);

		int n_sols = square_equation_solve(a, b, c, &x1, &x2);
		fputs(correct_ans, tempp);
		print_sq_eq_sols_fp(tempp, x1, x2, n_sols); //crash
	}
}


int is_str_all_space(char * str) {
	if (strlen(str) == 0) 
		return 1;

	int ch = str[0], i = 0, is = 1;
	
	while (ch != '\0') {
		if (!isspace(ch)) 
			is = 0;
		ch = str[++i];
	}
	return is;
}

void print_ascii_cat() {
	printf(""
"     /\\__/\\ " ENDL
"    /`    '\\ " ENDL
"  === 0  0 === " ENDL
"    \\  --  / " ENDL
"   /        \\ " ENDL
"  /          \\ " ENDL
" |            | " ENDL
"  \\  ||  ||  / " ENDL
"   \\_oo__oo_/#######o" ENDL);
}


int is_all_next_input_space() {
	int ch = 0, i = 0;

	while (ch != EOF && ch != '\n') {
		ch = getchar();
		if (!isspace(ch))
			++i;
	}
	if (!i)
		return 1;
	return 0;
}


int double_is_zero(double x) {
	if (fabs(x) < epsilon)
		return 1;
	return 0;
}