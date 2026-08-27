#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"


int main() //TODO: argc argv
{
    int cmd = 0; // command

    printf("Possible commands:\n"
           "\'q\': quit\n"
           "\'s\': solve equation\n"
           "\'f\': test format from file\n"
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