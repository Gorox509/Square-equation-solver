/**
 * \file
 * 
 * \brief File containing functions for plotting given second term polynomials
 */


#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"


/**
 * \defgroup Plotting Functions to plot parabolas
 * \brief Functions to plot 2-term polynomial in console or in file
 */


/**
 * \ingroup UserInterface
 * 
 * \brief Function that interacts with user and acknowledge information for plot
 * 
 * Asks for place to draw (file name or in console), amount of polynomials, canvas parameters, 
 * computes needed information and passes it to drawer functions.
 * 
 * \return Global constant code of success or error in function
 */
int plot_interactive() {
    printf("Enter file to write to (nothing to write on screen): ");
    char filename[MAX_STR_LEN] = "";
    fgets(filename, MAX_STR_LEN, stdin);

    replace_newline_with_null_terminator(filename);

    printf("Enter amount of plots: ");
    int plots_amount = 0, height = 0, width = 0;
    if (scanf("%d", &plots_amount) != 1) {
        printf(RED "Error while reading plots amount" BASE_FMT ENDL);
        clear_buffer();
        return GENERAL_ERROR;
    }

    if (isinf(plots_amount) or plots_amount <= 0) {
        printf("Wrong plots amount\n");
        return GENERAL_ERROR;
    }

    printf("Enter height & width of canvas (50x50 is recommended): ");
    if (scanf("%d %d", &height, &width) != 2) {
        printf(RED "Error while canvas parameters reading" BASE_FMT ENDL);
        clear_buffer();
        return GENERAL_ERROR;
    }

    if (isinf(height) || isinf(width) || height <= 0 || width <= 0) {
        printf("Wrong canvas parameters\n");
        return GENERAL_ERROR;
    }

    square_equation data[MAX_ARRAY_LEN]; // stores functions to plot
    for (int i = 0; i < plots_amount; ++i)
        input_compute_data_for_plot(&data[i]); // input and solve for every function

    double scale = NAN;
    // printf("Enter scale (x symbols per 1x1 square): ");
    // scanf("%lg", &scale);
    if (is_str_all_space(filename)) { // plot in console
        plot(stdout, data, plots_amount, 0, height, width);

    } else { // plot in file with given filename
        FILE *fp = fopen(filename, "w");
        plot(fp, data, plots_amount, 0, height, width);
        fclose(fp);
    }
    return CORRECT;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that interacts inputs and computes needed for plots data
 * 
 * \param [in, out] data pointer to square equation structure that's being read and written to
 * 
 * \return Global constant code of success or error in function
 */
int input_compute_data_for_plot(square_equation * data) {
    int input_err = input_square_coefs(&data->a, &data->b, &data->c);

	if (input_err == GENERAL_ERROR) {
		return GENERAL_ERROR;
	}
    int roots_code = square_equation_solve(data->a, data->b, data->c, &data->x1, &data->x2);
    data->roots_code = roots_code;
    return CORRECT;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that requests plotting the equations
 * 
 * File pointer is checked to be valid. If check fails, code of incorrect run is returned.
 * 
 * \param [out] fp pointer to file stream in which plot is printed
 * \param data array of square equation structures containing information to plot them
 * \param plots_amount amount of plots requested to draw
 * \param scale scale in which plot should be drawn
 * \param height height of canvas
 * \param width width of canvas
 * 
 * \return Global constant code of success or error in function
 */
int plot(FILE * fp, square_equation data[], int plots_amount, double scale, int height, int width) {
    if (isinf(scale) || height >= MAX_CANVAS_HEIGHT || width >= MAX_CANVAS_WIDTH) {
        printf(RED "Error: invalid scale" BASE_FMT ENDL);
        return INCORRECT;
    }
    if (fp == NULL) {
        printf(RED "Error: wrong file" BASE_FMT ENDL);
        return INCORRECT;
    }
    plot_squares(fp, data, plots_amount, scale, height, width);
    return CORRECT;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that prints symbols in buffer and requests printing of it
 * 
 * File pointer have be valid.
 * 
 * \param [out] fp pointer to file stream in which plot is printed
 * \param data array of square equation structures containing information to plot them
 * \param plots_amount amount of plots requested to draw
 * \param scale scale in which plot should be drawn
 * \param height height of canvas
 * \param width width of canvas
 */
void plot_squares(FILE * fp, square_equation data[], int plots_amount, double scale, int height, int width) {

    assert(fp != NULL);

    //Point vertex = {0, 0};
    //get_parabola_vertex(data, &vertex);
    height += height % 2; // to even number due to ploting format reasons
    width += width % 2;
    
    char * canvas[MAX_CANVAS_HEIGHT] = {0};
	for (int i = 0; i < height + 1; ++i) {
		canvas[i] = (char *) calloc(width + 1, sizeof(char));
    }

    PointInt current_position = {.x = -width / 2, .y = height / 2}; // x is to the right, y is upwards, starting in upper left position

    for ( ; current_position.y > -height / 2; --current_position.y) {
        for ( ; current_position.x < width / 2; ++current_position.x) {
            int symbol = ' ';
            symbol = pick_symbol_to_draw_for_plots(current_position, data, plots_amount, scale);
            canvas[current_position.y + height / 2][current_position.x + width / 2] = (char) symbol;
        }
        current_position.x = -width / 2; // back to 0th index of canvas line 
        canvas[current_position.y + height / 2][width] = '\n';
    }
    print_canvas_to_file(fp, height, width, canvas, scale);

    for (int i = 0; i < MAX_CANVAS_HEIGHT; ++i) {
		free(canvas[i]);
	}
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that prints canvas from buffer to file
 * 
 * File pointer is checked to be valid. If check fails, code of incorrect run is returned.
 * 
 * \param [out] fp pointer to file stream in which plot is printed
 * \param [out] canvas buffer, two-dimensional array of characters that contains symbols for drawing
 * \param scale scale in which plot should be drawn
 * \param height height of canvas
 * \param width width of canvas
 */
void print_canvas_to_file(FILE * fp, int height, int width, char * canvas[MAX_CANVAS_HEIGHT], double scale) {

    assert(fp != NULL);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width + 1; ++j) { //width + 1 for \n symbol at the end of each line
            fprintf(fp, "%2c", canvas[height - i][j]);
        }
    }
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that picks symbol to draw considering current position
 * 
 * \param [in, out] current_position integer point structure containing current position to put symbol in
 * \param data array of square equation structures containing information to plot them
 * \param plots_amount amount of plots requested to draw
 * \param scale scale in which plot should be drawn
 * 
 * \return symbol picked to draw
 */
int pick_symbol_to_draw_for_plots(PointInt current_position, square_equation data[], int plots_amount, double scale) {
    int symbol = ' ';
    for (int i = 0; i < plots_amount; ++i) {
        if (is_this_square_zero(current_position, scale)) {
            symbol = '0';
            break;
            }
        else if (is_parabola_in_this_square(data[i], current_position, scale)) {
            symbol = '#';
            break;
            }
        else if (is_this_square_on_x_axis(current_position, scale) && symbol == ' ') {
            symbol = '-';
            }
        else if (is_this_square_on_y_axis(current_position, scale) && symbol == ' ') {
            symbol = '|';
        }
    }
    return symbol;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that puts coordinates of parabola vertex in point structure
 * 
 * \param data array of square equation structures containing information to plot them
 * \param [out] vertex point structure in which coordinates of parabola vertex are put into
 */
void get_parabola_vertex(square_equation data, Point * vertex) {
    if (data.roots_code != TWO_ROOTS_CODE || data.roots_code != NO_ROOTS_CODE) {
        printf(RED "Error: cant calculate vertex" BASE_FMT ENDL);
    }
    vertex->x = (0 - data.b) / (2 * data.a);
    vertex->y = data.a * vertex->x * vertex ->x + data.b * vertex->x + data.c;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that requests plotting the equations
 * 
 * Checks if all corners of current square are on the same side of given parabola, if true, parabola is not in this square.
 * 
 * \param data square equation structure containing information of it
 * \param p integer point which corresponding square is checked
 * \param scale scale in which plot should be drawn
 * 
 * \return TRUE if parabola is in this square, FALSE otherwise
 */
bool is_parabola_in_this_square(square_equation data, PointInt p, double scale) {
    Point 
    lu_angle = {p.x - 0.5, p.y - 0.5}, 
    ld_angle = {p.x - 0.5, p.y + 0.5}, 
    ru_angle = {p.x + 0.5, p.y - 0.5}, 
    rd_angle = {p.x + 0.5, p.y + 0.5}; //TODO: code style - ?? ok

    if (abs(  parabola_lower_or_higher_than_point(data, lu_angle)
            + parabola_lower_or_higher_than_point(data, ld_angle)         // if continuous function doesn't cross that square, 
            + parabola_lower_or_higher_than_point(data, ru_angle)         // all angles must be at the same side of it (lower or higher)
            + parabola_lower_or_higher_than_point(data, rd_angle)) == 4)              
    {
        return FALSE;   
    }
    return TRUE;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that calculates on which side of parabola given point is
 * 
 * \param data square equation structure containing information of it
 * \param p point which coordinates is used in computation
 * 
 * \return 1 if parabola is lower than point, -1 if higher
 */
int parabola_lower_or_higher_than_point(square_equation data, Point p) {
    double true_y = data.a * p.x*p.x + data.b * p.x + data.c;
    if (true_y < p.y)
        return 1;
    return -1;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that calculates if current square is zero on cartesian coordinates
 * 
 * \param p integer point which corresponding square is checked
 * \param scale scale in which plot should be drawn
 * 
 * \return TRUE if current square is zeroth, FALSE otherwise
 */
bool is_this_square_zero(PointInt p, double scale) {
    if (p.x == 0 and p.y == 0)
        return TRUE;
    return FALSE;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that calculates if current square is on horisontal axis
 * 
 * \param p integer point which corresponding square is checked
 * \param scale scale in which plot should be drawn
 * 
 * \return TRUE if current square is on horisontal axis, FALSE otherwise
 */
bool is_this_square_on_x_axis(PointInt p, double scale) {
    if (p.y == 0)
        return TRUE;
    return FALSE;
}


/**
 * \ingroup Plotting
 * 
 * \brief Function that calculates if current square is on vertical axis
 * 
 * \param p integer point which corresponding square is checked
 * \param scale scale in which plot should be drawn
 * 
 * \return TRUE if current square is on vertical axis, FALSE otherwise
 */
bool is_this_square_on_y_axis(PointInt p, double scale) {
    if (p.x == 0)
        return TRUE;
    return FALSE;
}