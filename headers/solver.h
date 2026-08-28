/**
 * \file
 * 
 * \brief Header file for solver.cpp file
 * 
 * Contains prototypes of functions used
 */


#ifndef SOLVER_H

#define SOLVER_H

int square_equation_solve(double a, double b, double c, double * x1, double * x2);
int linear_equation_solve(double k, double b, double * x);
int print_square_equation_sols_stdout(double x1, double x2, int n);
int print_square_equation_sols_to_file(FILE * fp, double x1, double x2, int n_sol);
int print_square_equation_sols_to_str(char str[], double x1, double x2, int n_sol);
int input_square_coefs(double * a, double * b, double * c);
int square_equation_interactive();

#endif