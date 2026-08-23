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


const int MAX_STR_LEN = 100;
//---------------------------
enum escape_codes {       //|
ANY_NUM_CODE = -3,		  //|
TWO_ROOTS_CODE = -15,	  //| // todo real enum - done
ONE_ROOT_CODE = 9,	      //|
NO_ROOTS_CODE = -30,	  //|
WRONG_COEFS_CODE = -1,	  //|
};						  //|
//---------------------------
const int CORRECT = 1;
const int INCORRECT = 0;
const int GENERAL_ERROR = -13;


void create_n_tests_to_file(FILE * fp, int n, int n_roots_needed);
void create_test_two_roots_to_file(FILE * fp);
void round_to_n_digits(double * x, int n);
void create_test_one_root_to_file(FILE * fp);
double rand_double();


int main() {
    int n_tests_do_create = 0, n_roots_needed = 0;
    printf("Enter number of tests to create: ");
    scanf("%d", &n_tests_do_create);

    char filename[MAX_STR_LEN] = "";
    printf("Enter file name to add tests to: ");
    if (scanf("%s", filename) != 1) {
		printf(RED "Error during filename reading" BASE_FMT ENDL);
		return GENERAL_ERROR;
	}

    printf("Enter amount of roots needed: ");
    scanf("%d", &n_roots_needed);

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
    }
}


void create_test_two_roots_to_file(FILE * fp) {
    double x1 = rand_double(), x2 = rand_double();
    double a = rand_double();

    double b = (0 - a) * (x1 + x2);
    double c = a * x1 * x2;

    fprintf(fp, "%lg %lg %lg %d %lg %lg\n", a, b, c, TWO_ROOTS_CODE, x1, x2);
}

//todo line - done


void create_test_one_root_to_file(FILE * fp) {
    double x = rand_double();
    double k = rand_double();

    double b = (0 - k) * x;

    fprintf(fp, "0 %lg %lg %d %lg\n", k, b, ONE_ROOT_CODE, x);
}

void round_to_n_digits(double * x, int n) {
    *x = round(*x * pow(10, n)) / pow(10, n);
}


double rand_double() {
	return (((double) rand() - (double) rand()) / (double) rand());
}