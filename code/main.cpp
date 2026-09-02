/**
 * \file
 * 
 * \brief Main file that is used for processing user commands
 */

#include <getopt.h>
#include "../headers/defines.h"

#include "../headers/main.h"
#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"


#include "solver.cpp"
#include "testing.cpp"
#include "plot.cpp"
#include "util_funcs.cpp"

/**
 * \ingroup UserInterface
 * 
 * \brief main function that acts as menu for user interface
 * 
 * User is offered to input a symbol that's choosing what features should activate.
 * There's a testing mode that is activated with -t flag when executing the executable file
 * 
 * \param argc amount of used console arguments
 * \param argv strings containing the console arguments used. First value is always the command used to execute the file
 * 
 * \return CORRECT code if program ends successfully, various error codes in case of error
 */
int main(int argc, char *argv[]) //TODO: argc argv
{
    int cmd = 0;
    bool testing_mode = FALSE; 

    get_options(argc, argv, &testing_mode);

    printf("Possible commands:\n"
           "\'q\': quit\n"
           "\'s\': solve equation\n"
           "\'p\': plot the equation\n");

    if (testing_mode) {
        printf("\'f\': test format from file\n"
               "\'t\': test solver from file\n");
    }

    printf("Enter command: ");

    while ((cmd = getchar()) != EOF) // write next symbol
    {
        if (isspace(cmd)) //skip spaces before actual input
            continue;

        if (!is_next_input_all_space()) //TODO: name - done
            cmd = 0;                   // check if there's more characters after first
                                       // if true, default case happens

        switch (cmd)
        {
        case 'q':
            return CORRECT; // quit

        case 'f':
            if (testing_mode) 
            {
            format_test_interactive_from_file();
            }

            else
            {
                printf(RED "Access denied. Turn on testing mode" BASE_FMT ENDL);
            }
            break;

        case 't':
        {
            if (testing_mode)
            {
            char filename[MAX_STR_LEN] = "";
            printf("Enter test file name: ");
            if (scanf("%s", filename) != 1)
            {
                printf(RED "Error during filename reading" BASE_FMT ENDL);
                clear_buffer();
                break;
            }
            test_solver_from_file(filename);
            }

            else
            {
                printf(RED "Access denied. Turn on testing mode" BASE_FMT ENDL);
            }
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


/**
 * \ingroup UserInterface
 * 
 * \brief Function that is used to get console arguments and pass them to the variables
 * 
 * Argv have to be valid pointer.
 * In case of wrong options, prints error message and exits the program
 * 
 * \param argc amount of used console arguments
 * \param argv strings containing the console arguments used. First value is always the command used to execute the file
 * \param [in] testing_mode pointer to the variable containing the state of testing mode
 */
void get_options(int argc, char *argv[], bool *testing_mode) {

    assert(argv != NULL);
    assert(testing_mode != NULL);

    int opt = 0;
    while ((opt = getopt(argc, argv, "t")) != -1) {
        switch (opt) 
        {
            case 't':
                *testing_mode = TRUE;
                break;

            default:
                fprintf(stderr, RED "Fatal error: wrong opitons" BASE_FMT ENDL);
                exit(INCORRECT);
        }
    } //TODO: getopt && getopt_long - done
}