#ifndef PLOT_H

#define PLOT_H

int input_compute_data_for_plot(square_equation * data);
void plot(square_equation data[], int plots_amount, double scale, int height, int width);
void get_parabola_vertex(square_equation equation, Point * vertex);
void plot_squares(square_equation data[], int plots_amount, double scale, int height, int width);
int is_parabola_in_this_square(square_equation data, PointInt p, double scale);
int is_this_square_zero(PointInt p, double scale);
int is_this_square_on_x_axis(PointInt p, double scale);
int is_this_square_on_y_axis(PointInt p, double scale);
int is_parabola_lower_than_point(square_equation data, Point p);
int pick_symbol_to_draw_for_plots(PointInt current, square_equation data[], int plots_amount, double scale);

#endif