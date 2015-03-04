#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <stdlib.h>
#include <random>
#include <time.h>
#include <math.h>

#include "info.h"
#include "RuleGenerator.h"

#define SMOOTH_MAX RAND_MAX

#define R_I 7
#define R_A 21
#define ALPHA 32767
#define B1 2345
#define B2 9822
#define D1 10000
#define D2 20000

#define E_FIXED (2.71828*32768)



class Board {
    private:
        int *board;
        int *board_buffer;
        double *born;
        double *stay_alive;
        int density;
        int num_gliders;
        int update_algorithm;
        bool changing_background;
        bool pause;
        RuleGenerator gen;


        std::mt19937 e2;
        std::uniform_real_distribution<double> dist;

        int* get_circle(int x, int y, int r);

        void update_board_normal();
        void update_board_smooth();
        long s1(long x, long a);
        long s2(long x, long a, long b);
        long sm(long x, long y, long m);
        long s(long n, long m);
    public:
        Board();
        ~Board();
        void init_board();
        void init_quadrants();
        void init_center_dot();
        void init_gliders();
        void init_circle();
        void init_smooth_life();

        void make_glider(int x, int y, int orientation);
        void clear_board();

        void update_board();


        int get_num_alive_neighbors(int x, int y);

        void update_colors();

        int* get_board();

        void set_rules_to_life();
        void randomize_rules();
        void randomize_rules_non_deterministic();
        void set_density(int new_density);
        void set_update_algorithm(int new_algorithm);
        void modify_gliders(int factor);
        void toggle_changing_background();
        void toggle_pause();

        void rules_pretty();
        void rules_not_pretty();

        void rules_pretty_float();
        void rules_not_pretty_float();
};


#endif // BOARD_H_INCLUDED
