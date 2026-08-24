#include "defines.h"



#include "main.h"
#include "testing.h"
#include "plot.h"



// сделать тесты - в процессе, макросы - есть
// enum  это круто фр фр
int main() {
	int cmd = 0; 

	printf("Possible commands:\n"
		   "\'q\': quit\n"
		   "\'s\': solve equation\n"
		   "\'f\': test format from file with\n"
		   "\'t\': test solver from file\n"
		   "\'p\': plot graph\n"
		   "Enter command: ");

	while ((cmd = getchar()) != EOF) {
		if (isspace(cmd))
			continue;

		if (!is_all_next_input_space())
			cmd = 0;

		switch (cmd) {
		case 'q':
			return CORRECT;

		case 'f':
			format_test_interactive_from_file();
			break;

		case 't': {
			char filename[MAX_STR_LEN] = "";
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

		case 'p': {
			int plots_amount = 0, height = 0, width = 0;
			printf("Enter amounts of plots: ");
			scanf("%d", &plots_amount);
			if (isinf(plots_amount) or plots_amount <= 0) {
				printf("Wrong plots amount\n");
				break;
			}
			printf("Enter height & width of canvas (50x50 is recommended): ");
			scanf("%d %d", &height, &width);
			if (isinf(height) || isinf(width) || height <= 0 || width <= 0) {
				printf("Wrong canvas parameters\n");
				break;
			}
			square_equation data[MAX_ARRAY_LEN];
			for (int i = 0; i < plots_amount; ++i) 
				input_compute_data_for_plot(&data[i]);
			double scale = NAN;
			//printf("Enter scale (x symbols per 1x1 square): ");
			//scanf("%lg", &scale);
			plot(data, plots_amount, 0, height, width);
			break;
		}

		case 'c':
			print_ascii_cat();
			break;

		default:
			printf(RED "Wrong input. Try again\n" BASE_FMT);
			break;
		}
		printf("Enter command: ");

	}
	return CORRECT;
}


int square_equation_solve(double a, double b, double c, double * x1, double * x2)	{ // ax^2 + bx + c = 0
	
	if (!isfinite(a) || !isfinite(b) || !isfinite(c) || x1 == NULL || x2 == NULL)
		return WRONG_COEFS_CODE;

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

	// todoimplement isinf() + isnan() - done
	else {
		*x1 = (0 - b - sqrt(D)) / (2 * a);
		*x2 = (0 - b + sqrt(D)) / (2 * a);
		return TWO_ROOTS_CODE;
	}
}

int linear_equation_solve(double k, double b, double * x) { // kx + b = 0
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


void print_sq_eq_sols_fp(FILE * fp, double x1, double x2, int n_sol) {
	if (fp == NULL) 
		exit(0);

	if (custom_isinf(x1) || custom_isinf(x2)) //can be NaN;
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
			char next[MAX_STR_LEN] = "";
			fgets(next, MAX_STR_LEN, stdin);
			if (strlen(next) == 0 || is_str_all_space(next))
				return CORRECT;
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

	return CORRECT;
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


int custom_isinf(double x) {
	unsigned char * p = (unsigned char *) (&x + 1); 
	unsigned char * p1 = p - 1;
	unsigned char * p2 = p - 2;
	
	if ((*p1 == 0x7f || *p1 == 0xff) && *p2 == 0xf0) 
		return 1;
	return 0;
}

int custom_isnan(double x) {
	unsigned char * p = (unsigned char *) (&x + 1); 
	unsigned char * p1 = p - 1;
	unsigned char * p2 = p - 2;
	
	if (*p1 == 0x7f && *p2 > 0xf0)
		return 1;
	return 0;
}
// todo plot in console
// todo read about split files