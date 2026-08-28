/**
 * \file
 * 
 * \brief Header file for all files in general
 * 
 * Contains prototypes of functions used
 */


#ifndef INCLUDES_H

#define INCLUDES_H

#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>

#endif


#ifndef DEFINES_H

#define DEFINES_H

const int MAX_STR_LEN = 100;    ///< Maximum possible string length
const int MAX_BUF_LEN = 300;    ///< Maximum possible buffer length
const int MAX_ARRAY_LEN = 100;  ///< Maximum possible array length

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BASE_FMT "\033[0m"

#define ENDL "\n"
//---------------------------
enum escape_codes {       //| ///< escape codes for representing amount of roots in square equation
ANY_NUM_CODE = -1,		  //|
TWO_ROOTS_CODE = 2,	      //| // todo real enum - done
ONE_ROOT_CODE = 1,	      //|
NO_ROOTS_CODE = 0,	      //|
WRONG_COEFS_CODE = -2,	  //|
};						  //|
//---------------------------
const int CORRECT = 1;   ///< Correct run code
const int INCORRECT = 0; ///< Incorrect run code


const int GENERAL_ERROR = -3;	///< General error code

const double epsilon = 1e-6; ///< error with which double calculations are made


/**
 * \brief Structure containing square equation information
 * 
 * \param a, b, c coefficients of equation in form of ax^2 + bx + c = 0
 * \param roots_code code of how many roots equation have
 * \param x1, x2 roots of the equation
 */
struct square_equation {
	double a, b, c;
	int roots_code;
	double x1, x2;
};


/**
 * \brief Strucure containing coordinates of 2-dimensional point
 * 
 * \param x, y cartesian coordinates of point
 */
struct Point {
    double x;
    double y;
};


/**
 * \brief Strucure containing coordinates of 2-dimensional integer point
 * 
 * \param x, y cartesian coordinates of integer point
 */
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