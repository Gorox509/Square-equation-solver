#include "../headers/defines.h"

#include "../headers/create_random_tests.h"


int main() {
    int n_tests_do_create = 0, n_roots_needed = 0;
    printf("Enter number of tests to create: ");
    if (scanf("%d", &n_tests_do_create) != 1) {
        printf(RED "Error while reading amount of tests to create" BASE_FMT ENDL);
        return GENERAL_ERROR;
    }

    char filename[MAX_STR_LEN] = "";
    printf("Enter file name to add tests to: ");
    if (scanf("%s", filename) != 1) {
		printf(RED "Error during filename reading" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

    printf("Enter amount of roots needed: ");
    if (scanf("%d", &n_roots_needed) != 1) {
        printf(RED "Error while reading amount of roots" BASE_FMT ENDL);
        return GENERAL_ERROR;
    }

    FILE * fp = fopen(filename, "w+");

    if (fp == NULL) {
        fclose(fp);
        return GENERAL_ERROR;
    }

    create_n_tests_to_file(fp, n_tests_do_create, n_roots_needed);
    fclose(fp);
    return CORRECT;
}


void create_n_tests_to_file(FILE * fp, int n, int n_roots_neeeded) {
    switch(n_roots_neeeded) {
    case 2: 
        for (int i = 0; i < n; ++i) 
            create_test_two_roots_to_file(fp);
        break;

    case 1:
        for (int i = 0; i < n; ++i)
            create_test_one_root_to_file(fp);
        break;

    default:
        printf(RED "Error: wrong roots code" BASE_FMT ENDL);
        break;
    }
}


void create_test_two_roots_to_file(FILE * fp) {
    double x1 = rand_double(), x2 = rand_double();
    double a = rand_double();

    double b = (0 - a) * (x1 + x2); // (0 - a) for b to not be -0 
    double c = a * x1 * x2;

    fprintf(fp, "%lg %lg %lg %d %lg %lg\n", a, b, c, TWO_ROOTS_CODE, x1, x2);
}

//todo line - done


void create_test_one_root_to_file(FILE * fp) {
    double x = rand_double();
    double k = rand_double();

    double b = (0 - k) * x; // (0 - k) for b to not be -0

    fprintf(fp, "0 %lg %lg %d %lg\n", k, b, ONE_ROOT_CODE, x);
}

void round_to_n_digits(double * x, int n) {
    *x = round(*x * pow(10, n)) / pow(10, n);
}


double rand_double() {
	return ((double) rand() - RAND_MAX / 2);
}