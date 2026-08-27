#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"

int square_equation_solve(double a, double b, double c, double *x1, double *x2) { // ax^2 + bx + c = 0
    if (!custom_isfinite(a) || !custom_isfinite(b) || !custom_isfinite(c)) //TODO: error output - done
        return WRONG_COEFS_CODE;

    if (x1 == NULL || x2 == NULL) 
        return GENERAL_ERROR; // todo check if x1 == x2 - done

    if (x1 == x2)
        return GENERAL_ERROR;

    *x1 = NAN;
    *x2 = NAN;


    if (is_double_zero(a))
        return linear_equation_solve(b, c, x1);

    double Discriminant = b * b - 4 * a * c; //TODO: discriminant - done

    if (Discriminant < 0)
        return NO_ROOTS_CODE;

    else if (is_double_zero(Discriminant))
    {
        *x1 = -b / (2 * a); //TODO: comments - done
        return ONE_ROOT_CODE;
    }

    // todoimplement isinf() + isnan() - done
    else
    {
        double Discriminant_sqrt = sqrt(Discriminant);
        *x1 = (-b - Discriminant_sqrt) / (2 * a);
        *x2 = (-b + Discriminant_sqrt) / (2 * a); //TODO: undublicate - done
        return TWO_ROOTS_CODE;
    }
}

int linear_equation_solve(double k, double b, double *x) { // kx + b = 0

    if (is_double_zero(k))
    {
        if (is_double_zero(b))
        {
            return ANY_NUM_CODE; // any number
        }
        return NO_ROOTS_CODE; // no solutions
    }

    *x = -b / k;
    return ONE_ROOT_CODE;
}

int print_square_equation_sols_stdout(double x1, double x2, int n_sol)
{
    return print_square_equation_sols_to_file(stdout, x1, x2, n_sol);
}

int print_square_equation_sols_to_file(FILE * fp, double x1, double x2, int n_sol)
{
    if (fp == NULL) {
        printf(RED "Error while file opening" BASE_FMT ENDL);
        return GENERAL_ERROR;
    }
    char output_str[MAX_STR_LEN] = "";
    int return_value = 0;

    return_value = print_square_equation_sols_to_str(output_str, x1, x2, n_sol);
    fprintf(fp, "%s", output_str);
    return return_value;
}

int print_square_equation_sols_to_str(char str[], double x1, double x2, int n_sol) {
    if (custom_isinf(x1) || custom_isinf(x2)) // can be NaN;
    {
        printf(RED "Error: infinite roots" BASE_FMT ENDL);
        return GENERAL_ERROR;
    }
    if (!custom_isnan(x1) && is_double_zero(x1))
        x1 = 0;
    
    if (!custom_isnan(x2) && is_double_zero(x2))
        x2 = 0;

    switch (n_sol)
    {
    case NO_ROOTS_CODE:
        sprintf(str, YELLOW "No solutions" BASE_FMT ENDL);
        break;

    case ONE_ROOT_CODE:
        sprintf(str, YELLOW "Solution: %lg" BASE_FMT ENDL, x1);
        break;

    case TWO_ROOTS_CODE:
        sprintf(str, GREEN "Solutions: %lg %lg" BASE_FMT ENDL, x1, x2);
        break;

    case ANY_NUM_CODE:
        sprintf(str, YELLOW "Any number" BASE_FMT ENDL);
        break;

    case WRONG_COEFS_CODE:
        sprintf(str, RED "Error: invalid coefficients" BASE_FMT ENDL);
        break;

    default:
        sprintf(str, RED "Error" BASE_FMT ENDL);
        return INCORRECT;
    }
    return CORRECT;
}

int input_square_coefs(double *a, double *b, double *c)
{

    printf("Enter coefs: ");

    for (int remaining_attempts = 10; remaining_attempts > -1; --remaining_attempts) 
    {

        if (scanf("%lg %lg %lg", a, b, c) == 3)
        {
            char next[MAX_STR_LEN] = "";
            fgets(next, MAX_STR_LEN, stdin);
            if (strlen(next) == 0 || non_space_symbols_in_str(next))
                return CORRECT;
        }

        if (remaining_attempts)
            printf(RED "Wrong input format. Try again: " BASE_FMT);

        clear_buffer();
    }
    printf(RED "Too many attempts.\n" BASE_FMT);
    return GENERAL_ERROR;
}

int square_equation_interactive()
{
    double a  = NAN, b  = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;
    int n_sol = 0, input_err = 0;

    input_err = input_square_coefs(&a, &b, &c);

    if (input_err == GENERAL_ERROR)
    {
        return GENERAL_ERROR;
    }

    n_sol = square_equation_solve(a, b, c, &x1, &x2);

    print_square_equation_sols_stdout(x1, x2, n_sol);

    return CORRECT;
}