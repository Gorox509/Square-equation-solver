#ifndef PLOT_H

#define PLOT_H

const int MAX_CANVAS_HEIGHT = 400;
const int MAX_CANVAS_WIDTH = 400;

int plot_interactive();
int input_compute_data_for_plot(square_equation * data);
int plot(FILE * fp, square_equation data[], int plots_amount, double scale, int height, int width);
void get_parabola_vertex(square_equation equation, Point * vertex);
void plot_squares(FILE * fp, square_equation data[], int plots_amount, double scale, int height, int width);
bool is_parabola_in_this_square(square_equation data, PointInt p, double scale);
bool is_this_square_zero(PointInt p, double scale);
bool is_this_square_on_x_axis(PointInt p, double scale);
bool is_this_square_on_y_axis(PointInt p, double scale);
int parabola_lower_or_higher_than_point(square_equation data, Point p);
int pick_symbol_to_draw_for_plots(PointInt current_position, square_equation data[], int plots_amount, double scale);
void print_canvas_to_file(FILE * fp, int height, int width, char canvas[MAX_CANVAS_HEIGHT][MAX_CANVAS_WIDTH], double scale);

#endif