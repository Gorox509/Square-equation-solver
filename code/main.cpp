#include "../headers/defines.h"

#include "../headers/main.h"
#include "../headers/plot.h"
#include "../headers/testing.h"


int main() //TODO: argc argv
{
    int cmd = 0; // command

    printf("Possible commands:\n"
           "\'q\': quit\n"
           "\'s\': solve equation\n"
           "\'f\': test format from file with\n"
           "\'t\': test solver from file\n"
           "\'p\': plot the equation\n"
           "Enter command: ");

    while ((cmd = getchar()) != EOF) // write next symbol
    {
        if (isspace(cmd)) //skip spaces before actual input
            continue;

        if (!n_spaces_in_next_input()) //TODO: name - done
            cmd = 0;                   // check if there's more characters after after first
                                       // if true, default case happens
        switch (cmd)
        {
        case 'q':
            return CORRECT; // quit

        case 'f':
            format_test_interactive_from_file();
            break;

        case 't':
        {
            char filename[MAX_STR_LEN] = "";
            printf("Enter test file name: ");
            if (scanf("%s", filename) != 1)
            {
                printf(RED "Error during filename reading" BASE_FMT ENDL);
                break;
            }
            test_solver_from_file(filename);
            break;
        }

        case 's':
            square_equation_interactive();
            break;

        case 'p': //TODO: to func() - done
        {
            plot_interactive();
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

int square_equation_solve(double a, double b, double c, double *x1, double *x2) { // ax^2 + bx + c = 0
    if (!isfinite(a) || !isfinite(b) || !isfinite(c)) //TODO: error output - done
        return WRONG_COEFS_CODE;

    if (x1 == NULL || x2 == NULL) 
        return GENERAL_ERROR;

    *x1 = NAN;
    *x2 = NAN;

    if (a < 0.) {
        a = fabs(a); // a is always positive to prevent -0 return
        b *= -1.;
        c *= -1;
    }

    if (double_is_zero(a))
        return linear_equation_solve(b, c, x1);

    double Discriminant = b * b - 4 * a * c; //TODO: discriminant - done

    if (Discriminant < 0)
        return NO_ROOTS_CODE;

    else if (double_is_zero(Discriminant))
    {                            // 0 - b to prevent unwanted -0 return, since if b is 0 (-b) is -0, and (0 - b) is 0
        *x1 = (0 - b) / (2 * a); //TODO: comments - done
        return ONE_ROOT_CODE;
    }

    // todoimplement isinf() + isnan() - done
    else
    {
        double Discriminant_sqrt = sqrt(Discriminant);
        *x1 = (0 - b - Discriminant_sqrt) / (2 * a);
        *x2 = (0 - b + Discriminant_sqrt) / (2 * a); //TODO: undublicate - done
        return TWO_ROOTS_CODE;
    }
}

int linear_equation_solve(double k, double b, double *x) { // kx + b = 0
    if (k < 0.)
    {
        k = fabs(k); // k is always positive to prevent -0 return
        b *= -1;
    }

    if (double_is_zero(k))
    {
        if (double_is_zero(b))
        {
            return ANY_NUM_CODE; // any number
        }
        return NO_ROOTS_CODE; // no solutions
    }

    *x = (0 - b) / k; // 0 - b to prevent unwanted -0 return, since if b is 0 (-b) is -0, and (0 - b) is 0
    return ONE_ROOT_CODE;
}

int print_square_equation_sols_stdout(double x1, double x2, int n_sol)
{
    return print_square_equation_sols_fp(stdout, x1, x2, n_sol);
}

int print_square_equation_sols_fp(FILE *fp, double x1, double x2, int n_sol)
{
    if (fp == NULL) {
        printf(RED "Error while file opening" BASE_FMT ENDL);
        return GENERAL_ERROR;
    }
        
    if (custom_isinf(x1) || custom_isinf(x2)) // can be NaN;
    {
        printf(RED "Error: infinite roots" BASE_FMT ENDL);
        return GENERAL_ERROR;
        //exit(0); // red flag - dealt with
    }

    switch (n_sol)
    {
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
            if (strlen(next) == 0 || is_str_all_space(next))
                return CORRECT;
        }

        if (remaining_attempts)
            printf(RED "Wrong input format. Try again: " BASE_FMT);

        clear_buffer();
    }
    printf(RED "Too many attempts.\n" BASE_FMT);
    return GENERAL_ERROR;
}

int clear_buffer()
{

    int ch = 0, n_cleared_chars = 0;

    while (ch != EOF && ch != '\n')
    {
        ch = getchar();
        ++n_cleared_chars;
    }
    return n_cleared_chars;
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

int is_str_all_space(char *str)
{
    if (strlen(str) == 0)
        return TRUE;

    int ch = str[0], iter = 0, answer = 1;

    while (ch != '\0')
    {
        if (!isspace(ch))
            answer = 0;
        ch = str[++iter];
    }
    return answer;
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

int n_spaces_in_next_input()
{
    int ch = 0, n_non_space_symbols = 0;

    while (ch != EOF && ch != '\n')
    {
        ch = getchar();
        if (!isspace(ch))
            ++n_non_space_symbols;
    }
    if (!n_non_space_symbols)
        return TRUE;
    return FALSE;
}

int double_is_zero(double x)
{
    if (fabs(x) < epsilon)
        return TRUE; //TODO: bool
    return FALSE;
}

int custom_isinf(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1);
    unsigned char *p1 = p - 1;
    unsigned char *p2 = p - 2;

    if ((*p1 == 0x7f || *p1 == 0xff) && *p2 == 0xf0)
        return TRUE;
    return FALSE;
}

int custom_isnan(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1); // to the end of number
    unsigned char *p1 = p - 1;                    // to the first byte
    unsigned char *p2 = p - 2;                    // to the second byte

    if (*p1 == 0x7f && *p2 > 0xf0)
        return TRUE;
    return FALSE;
}
// todo plot in console - done
// todo read about split files - done

void replace_newline_with_null_terminator(char str[MAX_STR_LEN])
{
    char *idx = strchr(str, '\n');
    *idx = '\0';
}

