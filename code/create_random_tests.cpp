/**
 * \file
 * 
 * \brief Main file used for generating random tests
 */


#include "../headers/defines.h"

#include "../headers/create_random_tests.h"


/**
 * \defgroup RandomTests Functions for creating random tests for solver
 * \brief Functions that are used to create random tests for solver
 */


/**
 * \ingroup RandomTests
 * 
 * \brief Function that interacts with user to do create random tests
 * 
 * Asks user to enter amount of tests to create, file to write them to and amount of roots in these tests
 * 
 * \return Global constant code of success or error in function
 */
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


/**
 * \ingroup RandomTests
 * 
 * \brief Function that creates requested amount of tests to file
 * 
 * File pointer have to be valid.
 * In case of wrong requested amount of roots prints error message and returns error code.
 * 
 * \param [out] fp pointer to the file stream to write tests to
 * \param n amount of roots to create
 * \param n_roots amount of roots tests should have
 */
void create_n_tests_to_file(FILE * fp, int n, int n_roots_neeeded) {

    assert(fp != NULL);

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


/**
 * \ingroup RandomTests
 * 
 * \brief Function that creates test line with 2 roots to file
 * 
 * File pointer have to be valid.
 * 
 * \param [out] fp pointer to the file stream to write tests to
 */
void create_test_two_roots_to_file(FILE * fp) {

    assert(fp != NULL);

    double x1 = rand_double(), x2 = rand_double();
    double a = rand_double();

    double b = (0 - a) * (x1 + x2); // (0 - a) for b to not be -0 
    double c = a * x1 * x2;

    fprintf(fp, "%lg %lg %lg %d %lg %lg\n", a, b, c, TWO_ROOTS_CODE, x1, x2);
}

//todo line - done


/**
 * \ingroup RandomTests
 * 
 * \brief Function that creates requested amount of tests to file
 * 
 * File pointer have to be valid.
 * 
 * \param [out] fp pointer to the file stream to write tests to
 */
void create_test_one_root_to_file(FILE * fp) {
    double x = rand_double();
    double k = rand_double();

    double b = (0 - k) * x; // (0 - k) for b to not be -0

    fprintf(fp, "0 %lg %lg %d %lg\n", k, b, ONE_ROOT_CODE, x);
}


/**
 * \ingroup RandomTests
 * 
 * \brief Function that creates random double numbers
 * 
 * \return random double number
 */
double rand_double() {
	return ((double) rand() - RAND_MAX / 2);
}