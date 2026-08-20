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

#define ANY_NUM_CODE -3


int square_equation_ptr(double a, double b, double c, double * x1, double * x2);

int linear_equation_ptr(double a, double b, double * x);

void print_sq_eq_sols(double x1, double x2, int n);

int input_square_coefs(double * a, double * b, double * c);

void input_go_to_next_line();




int main() {
	double a = 0., b = 0., c = 0.;
	double x1 = 0., x2 = 0.;
	int n_sol = 0, input_err = 0;

	input_err = input_square_coefs(&a, &b, &c);

	if (input_err)
		return -1;

	n_sol = square_equation_ptr(a, b, c, &x1, &x2);

	print_sq_eq_sols(x1, x2, n_sol);

	return 0;
}


int square_equation_ptr(double a, double b, double c, double * x1, double * x2)	{ // ax^2 + bx + c = 0

	if (!isfinite(a) || !isfinite(b) || !isfinite(c) || x1 == 0 || x2 == 0)
		return -1;

	const double epsilon = 1e-6;

	if (abs(a) < epsilon)
		return linear_equation_ptr(b, c, x1);

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

int linear_equation_ptr(double a, double b, double * x) { // ax + b = 0

	const double epsilon = 1e-6;

	if ((abs(a) < epsilon)) {
			if (abs(b) < epsilon)
				return ANY_NUM_CODE; // any number
			return 0; // no solutions
		}

	*x = -b / a;
	return 1;
}


void print_sq_eq_sols(double x1, double x2, int n_sol) {

	switch(n_sol) {
	case 0:
		printf(YELLOW "No solutions" BASE_FMT);
		break;

	case 1:
		printf(YELLOW "Solution: %lg" BASE_FMT, x1);
		break;

	case 2:
		printf(GREEN "Solutions: %lg %lg" BASE_FMT, x1, x2);
		break;

	case ANY_NUM_CODE:
		printf(YELLOW "Any number" BASE_FMT);
		break;

	case -1:
		printf(RED "Error: invalid coefficients" BASE_FMT);
		break;

	default:
		printf(RED "Error" BASE_FMT);
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

		input_go_to_next_line();
	}
	printf(RED "Too many attempts.\n" BASE_FMT);
	return -1;
}


void input_go_to_next_line() {

	int ch = 0;

	while (ch != EOF && ch != '\n')
		ch = getchar();
}

