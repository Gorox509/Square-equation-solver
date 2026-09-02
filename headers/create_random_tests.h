/**
 * \file
 * 
 * \brief Header file for create_random_tests.cpp file
 * 
 * Contains prototypes of functions used
 */


#ifndef RANDOM_TESTS_H

#define RANDOM_TESTS_H

void create_n_tests_to_file(FILE *fp, int n, int n_roots_needed);
void create_test_two_roots_to_file(FILE *fp);
void create_test_one_root_to_file(FILE *fp);
double rand_double();

#endif