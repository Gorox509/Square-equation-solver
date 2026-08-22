#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>




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



#include "testing.c"



// сделать тесты - в процессе, макросы - есть
// enum  это круто фр фр
int main() {
	int cmd = 0;

	printf("Possible commands:\n\'q\': quit\n\'s\': solve equation\n\'f\': test format from file with\n\'t\': test solver from file\nEnter command: ");

	while ((cmd = getchar()) != EOF) {
		if (clear_buffer() > 1)
			cmd = 0;

		switch (cmd) {
		case 'q':
			return 0;

		case 'f':
			format_test_interactive_from_file();
			getchar();
			break;

		case 't': {
			char filename[MAX_STR_SIZE] = "";
			printf("Enter test file name: ");
			if (scanf("%s", filename) != 1) {
				printf(RED "Error during filename reading" BASE_FMT ENDL);
				break;
			}
			test_solver_from_file(filename);
			getchar();
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

	const double epsilon = 1e-6;

	*x1 = NAN;
	*x2 = NAN;

	if (a < 0.) {
		a = fabs(a);
		b *= -1.;
		c *= -1;
	}

	if (fabs(a) < epsilon)
		return linear_equation_solve(b, c, x1);

	double D = b * b - 4 * a * c;

	if (D < 0)
		return NO_ROOTS_CODE;

	else if (fabs(D) < epsilon) {
		*x1 = (0 - b) / (2 * a);
		return ONE_ROOT_CODE;
	}

	else {
		*x1 = (0 - b - sqrt(D)) / (2 * a);
		*x2 = (0 - b + sqrt(D)) / (2 * a);
		return TWO_ROOTS_CODE;
	}
}

int linear_equation_solve(double k, double b, double * x) { // ax + b = 0

	const double epsilon = 1e-6;

	if (k < 0.) {
		k = fabs(k);
		b *= -1;
	}

	if ((fabs(k) < epsilon)) {
			if (fabs(b) < epsilon)
				return ANY_NUM_CODE; // any number
			return NO_ROOTS_CODE; // no solutions
		}

	*x = (0 - b) / k;
	return ONE_ROOT_CODE;
}


void print_sq_eq_sols_stdout(double x1, double x2, int n_sol) {
	print_sq_eq_sols_fp(stdout, x1, x2, n_sol);
}


void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol) {
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
	double a = 0., b = 0., c = 0.;
	double x1 = 0., x2 = 0.;
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

	int n = 0, m = 0, i = 0;

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
	
	while (1) {
		i++;
		char correct_ans[MAX_STR_SIZE], ans[MAX_STR_SIZE];

		if (fgets(correct_ans, MAX_STR_SIZE, fp) == NULL || fgets(ans, MAX_STR_SIZE, fp) == NULL) {
			printf("Wrong Answers: %d/%d\n", n, m);
			fclose(fp);
			remove(".temp");
			return 0;
		}

		if (!strcmp(ans, correct_ans)) {
			m++;
		}
		else {
			n++;
			m++;
			printf("Wrong Answer №%d.\nCorrect answer: %sYour answer: %s\n", i, correct_ans, ans);
		}
	}
}


int do_format_test_from_file(char * filename) {
	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		return GENERAL_ERROR;
	}

	FILE * tempp = fopen(".temp", "w+");
	while (1) {
		double a = 0., b = 0., c = 0., x1 = 0., x2 = 0.;

		char correct_ans[MAX_STR_SIZE] = "";

		int ch = 0;

		if ((ch = getc(fp)) == EOF) {
			fclose(tempp);
			fclose(fp);
			return 0;
		}
		ungetc(ch, fp);

		fscanf(fp, "%lg %lg %lg ", &a, &b, &c);
		fgets(correct_ans, MAX_STR_SIZE, fp);

		int n_sols = square_equation_solve(a, b, c, &x1, &x2);
		fputs(correct_ans, tempp);
		print_sq_eq_sols_fp(tempp, x1, x2, n_sols);
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