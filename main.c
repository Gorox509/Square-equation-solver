#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"

#define ANY_NUM_CODE -3
#define MAX_STR_SIZE 100


int square_equation_solve(double a, double b, double c, double * x1, double * x2);

int linear_equation_solve(double a, double b, double * x);

void print_sq_eq_sols_stdout(double x1, double x2, int n);

void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol);

int input_square_coefs(double * a, double * b, double * c);

int clear_buffer();

int sq_eq_interactive();

int test_interactive_from_file();

int do_test_from_file(char * filename);


int main() {
	int cmd = 0;

	printf("Possible commands:\n\'q\': quit\n\'s\': solve equation\n\'t\': test from file\nEnter command: ");

	while ((cmd = getchar()) != EOF) {
		if (clear_buffer() > 1)
			cmd = 0;

		switch (cmd) {
		case 'q':
			return 0;

		case 's':
			sq_eq_interactive();
			break;

		case 't':
			test_interactive_from_file();
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

	if (!isfinite(a) || !isfinite(b) || !isfinite(c) || x1 == 0 || x2 == 0)
		return -1;

	const double epsilon = 1e-6;

	if (abs(a) < epsilon)
		return linear_equation_solve(b, c, x1);

	double D = b * b - 4 * a * c;

	if (D < 0)
		return 0;

	else if (abs(D) < epsilon) {
		*x1 = (-b) / (2 * a);
		return 1;
	}

	else {
		*x1 = (-b - sqrt(D)) / (2 * a);
		*x2 = (-b + sqrt(D)) / (2 * a);
		return 2;
	}
}

int linear_equation_solve(double a, double b, double * x) { // ax + b = 0

	const double epsilon = 1e-6;

	if ((fabs(a) < epsilon)) {
			if (fabs(b) < epsilon)
				return ANY_NUM_CODE; // any number
			return 0; // no solutions
		}

	*x = -b / a;
	return 1;
}


void print_sq_eq_sols_stdout(double x1, double x2, int n_sol) {
	print_sq_eq_sols_fp(stdout, x1, x2, n_sol);
}


void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol) {
	switch(n_sol) {
	case 0:
		fprintf(fp, YELLOW "No solutions" BASE_FMT ENDL);
		break;

	case 1:
		fprintf(fp, YELLOW "Solution: %lg" BASE_FMT ENDL, x1);
		break;

	case 2:
		fprintf(fp, GREEN "Solutions: %lg %lg" BASE_FMT ENDL, x1, x2);
		break;

	case ANY_NUM_CODE:
		fprintf(fp, YELLOW "Any number" BASE_FMT ENDL);
		break;

	case -1:
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

		if (scanf("%lg %lg %lg", a, b, c) == 3)
			return 0;

		if (attempts)
			printf(RED "Wrong input format. Try again: " BASE_FMT);

		clear_buffer();
	}
	printf(RED "Too many attempts.\n" BASE_FMT);
	return -1;
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

	if (input_err) {
		return -1;
	}

	n_sol = square_equation_solve(a, b, c, &x1, &x2);

	print_sq_eq_sols_stdout(x1, x2, n_sol);

	clear_buffer();

	return 0;
}


int test_interactive_from_file() {
	char filename[MAX_STR_SIZE] = "";

	int n = 0, m = 0;

	printf("Enter test file name: ");

	if (scanf("%s", filename) != 1) {
		printf("Error during filename reading\n");
		return -1;
	}

	int test_err = do_test_from_file(filename);

	if (test_err == -1) {
		printf("Error: no such file\n");
		remove(".temp"); //todo просто файл без временных файлов
		return -1;
	}

	FILE * fp = fopen(".temp", "r");

	while (1) {
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
			printf("Wrong Answer.\nCorrect answer: %sYour answer: %s", correct_ans, ans);
		}
	}
}


int do_test_from_file(char * filename) {
	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		return -1;
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
