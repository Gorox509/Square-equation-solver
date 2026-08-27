#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>

#ifndef DEFINES_H

#define DEFINES_H



const int MAX_STR_LEN = 100;
const int MAX_BUF_LEN = 300;
const int MAX_ARRAY_LEN = 100;

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"
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

const double epsilon = 1e-6;

struct square_equation {
	double a, b, c;
	int roots_code;
	double x1, x2;
};

struct Point {
    double x;
    double y;
};


struct PointInt {
    int x;
    int y;
};


#endif


#ifndef TRUE

#define TRUE 1

#endif

#ifndef FALSE

#define FALSE 0

#endif