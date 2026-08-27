#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/solver.h"
#include "../headers/util_funcs.h"

int plot_interactive() {
    printf("Enter file to write to (nothing to write on screen): ");
    char filename[MAX_STR_LEN] = "";
    fgets(filename, MAX_STR_LEN, stdin);

    replace_newline_with_null_terminator(filename);

    printf("Enter amounts of plots: ");
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

int input_compute_data_for_plot(square_equation * data) {
    int input_err = input_square_coefs(&data->a, &data->b, &data->c);

	if (input_err == GENERAL_ERROR) {
		return GENERAL_ERROR;
	}
    int roots_code = square_equation_solve(data->a, data->b, data->c, &data->x1, &data->x2);
    data->roots_code = roots_code;
    return CORRECT;
}

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


void plot_squares(FILE * fp, square_equation data[], int plots_amount, double scale, int height, int width) {
    //Point vertex = {0, 0};
    //get_parabola_vertex(data, &vertex);
    height += height % 2; // to even number due to ploting format reasons
    width += width % 2;
    
    int canvas[MAX_CANVAS_HEIGHT][MAX_CANVAS_WIDTH] = {0};

    PointInt current_position = {.x = -width / 2, .y = height / 2}; // x is to the right, y is upwards, starting in upper left position

    for ( ; current_position.y > -height / 2; --current_position.y) {
        for ( ; current_position.x < width / 2; ++current_position.x) {
            int symbol = ' ';
            symbol = pick_symbol_to_draw_for_plots(current_position, data, plots_amount, scale);
            canvas[current_position.y + height / 2][current_position.x + width / 2] = symbol;
        }
        current_position.x = -width / 2; // back to 0th index of canvas line 
        canvas[current_position.y + height / 2][width] = '\n';
    }
    print_canvas_to_file(fp, height, width, canvas, scale);
}


void print_canvas_to_file(FILE * fp, int height, int width, int canvas[MAX_CANVAS_HEIGHT][MAX_CANVAS_WIDTH], double scale) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width + 1; ++j) { //width + 1 for \n symbol at the end of each line
            fprintf(fp, "%2c", canvas[height - i][j]);
        }
    }
}


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


void get_parabola_vertex(square_equation data, Point * vertex) {
    if (data.roots_code != TWO_ROOTS_CODE || data.roots_code != NO_ROOTS_CODE) {
        printf(RED "Error: cant calculate vertex" BASE_FMT ENDL);
    }
    vertex->x = (0 - data.b) / (2 * data.a);
    vertex->y = data.a * vertex->x * vertex ->x + data.b * vertex->x + data.c;
}


int is_parabola_in_this_square(square_equation data, PointInt p, double scale) {
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


int parabola_lower_or_higher_than_point(square_equation data, Point p) {
    double true_y = data.a * p.x*p.x + data.b * p.x + data.c;
    if (true_y < p.y)
        return 1;
    return -1;
}


int is_this_square_zero(PointInt p, double scale) {
    if (p.x == 0 and p.y == 0)
        return TRUE;
    return FALSE;
}

int is_this_square_on_x_axis(PointInt p, double scale) {
    if (p.y == 0)
        return TRUE;
    return FALSE;
}

int is_this_square_on_y_axis(PointInt p, double scale) {
    if (p.x == 0)
        return TRUE;
    return FALSE;
}