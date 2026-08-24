#include "defines.h"
#include "plot.h"
#include "main.h"


int input_compute_data_for_plot(square_equation * data) {
    int input_err = input_square_coefs(&data->a, &data->b, &data->c);

	if (input_err == GENERAL_ERROR) {
		return GENERAL_ERROR;
	}
    int roots_code = square_equation_solve(data->a, data->b, data->c, &data->x1, &data->x2);
    data->roots_code = roots_code;
    return CORRECT;
}

void plot(square_equation data[], int plots_amount, double scale, int height, int width) {
    if (isinf(scale)) {
        printf(RED "Error: invalid scale" BASE_FMT ENDL);
    }
    plot_squares(data, plots_amount, scale, height, width);
}


void plot_squares(square_equation data[], int plots_amount, double scale, int height, int width) {
    //Point vertex = {0, 0};
    //get_parabola_vertex(data, &vertex);
    PointInt current = {.x = -width / 2, .y = height / 2}; // x is to the right, y is upwards, starting in upper left position
    for ( ; current.y > -height / 2; --current.y) {
        for ( ; current.x < width / 2; ++current.x) {
            int symbol = ' ';
            symbol = pick_symbol_to_draw_for_plots(current, data, plots_amount, scale);
            printf("%2c", symbol);
        }
        current.x = -width / 2;
        printf(ENDL);
    }
}


int pick_symbol_to_draw_for_plots(PointInt current, square_equation data[], int plots_amount, double scale) {
    int symbol = ' ';
    for (int i = 0; i < plots_amount; ++i) {
        if (is_this_square_zero(current, scale)) {
            symbol = '0';
            break;
            }
        else if (is_parabola_in_this_square(data[i], current, scale)) {
            symbol = '#';
            break;
            }
        else if (is_this_square_on_x_axis(current, scale) && symbol == ' ') {
            symbol = '-';
            }
        else if (is_this_square_on_y_axis(current, scale) && symbol == ' ') {
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
    Point lu_angle = {p.x - 0.5, p.y - 0.5}, ld_angle = {p.x - 0.5, p.y + 0.5}, ru_angle = {p.x + 0.5, p.y - 0.5}, rd_angle = {p.x + 0.5, p.y + 0.5};

    if (abs(is_parabola_lower_than_point(data, lu_angle) + is_parabola_lower_than_point(data, ld_angle)
          + is_parabola_lower_than_point(data, ru_angle) + is_parabola_lower_than_point(data, rd_angle)) == 4) 
    {
        return 0;   
    }
    return 1;
}


int is_parabola_lower_than_point(square_equation data, Point p) {
    double true_y = data.a * p.x*p.x + data.b * p.x + data.c;
    if (true_y < p.y)
        return 1;
    return -1;
}



int is_this_square_zero(PointInt p, double scale) {
    if (p.x == 0 and p.y == 0)
        return 1;
    return 0;
}

int is_this_square_on_x_axis(PointInt p, double scale) {
    if (p.y == 0)
        return 1;
    return 0;
}

int is_this_square_on_y_axis(PointInt p, double scale) {
    if (p.x == 0)
        return 1;
    return 0;
}