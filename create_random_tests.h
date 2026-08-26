#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>

#ifndef RANDOM_TESTS_H

#define RANDOM_TESTS_H

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

#endif