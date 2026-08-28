/**
 * \file
 * 
 * \brief Main file that is used for processing user commands
 */


#include "../headers/defines.h"

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
int main(int argc, char * argv[]) //TODO: argc argv
{
    int cmd = 0;
    int testing_mode = FALSE; 

    printf("Possible commands:\n"
           "\'q\': quit\n"
           "\'s\': solve equation\n"
           "\'p\': plot the equation\n");
           

    if (argc >= 2 && !strcmp(argv[1], "-t")) {
        testing_mode = TRUE;
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
            format_test_interactive_from_file();
            break;

        case 't':
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