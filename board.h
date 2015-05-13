#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED


#include <stdlib.h>
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "info.h"
#include "RuleGenerator.h"

#define SMOOTH_MAX RAND_MAX

#define R_I 7
#define R_A 21
#define ALPHA_N 0.028
#define ALPHA_M 0.147
#define B1 0.2
#define B2 0.365
#define D1 0.30
#define D2 0.549

#define PI 3.14159265359



class Board {
    private:
        int *board;
        int *board_buffer;
        double *board_float;
        double *board_buffer_float;
        double *born;
        double *stay_alive;
        double *smooth_rules;
        int density;
        int num_gliders;
        int update_algorithm;
        bool changing_background;
        bool pause;
        RuleGenerator gen;

        double r_i;
        double r_a;
        double alpha_n;
        double alpha_m;
        double b1;
        double b2;
        double d1;
        double d2;

        std::mt19937 e2;
        std::uniform_real_distribution<double> dist;

        void get_circle(int x, int y, int r, int* points);
        void get_polygon(int x, int y, int mag, int dim, float irreg, int* points);

        void get_mathy_triangle2(int x, int y, int mag, int* points);

        void update_board_normal();
        void update_board_smooth();
        double s1(double x, double a, double alpha);
        double s2(double x, double a, double b);
        double sm(double x, double y, double m);
        double s(double n, double m);
    public:
        Board();
        ~Board();
        void init_board();
        void init_quadrants();
        void init_center_dot();
        void init_gliders();
        void init_circle();
        void init_triangle();
        void init_smooth_life();
        void init_square_shell();
        void init_circle_shell();
        void init_triangle_shell();

        void make_glider(int x, int y, int orientation);
        void clear_board();

        void update_board();


        int get_num_alive_neighbors(int x, int y);

        void update_colors();

        int* get_board();
        double *get_board_float();

        void set_rules_to_life();
        void randomize_rules();
        void randomize_rules_non_deterministic();
        void randomize_rules_smooth();

        void set_density(int new_density);
        void set_update_algorithm(int new_algorithm);
        void modify_gliders(int factor);
        void toggle_changing_background();
        void toggle_pause();

        void rules_pretty();
        void rules_not_pretty();

        void rules_pretty_float();
        void rules_not_pretty_float();

        void print_rules();
};


#endif // BOARD_H_INCLUDED
