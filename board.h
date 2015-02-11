#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <stdlib.h>

#include "info.h"
#include "RuleGenerator.h"

class Board {
    private:
        int *board;
        int *board_buffer;
        int *born;
        int *stay_alive;
        int density;
        int num_gliders;
        bool changing_background;
        bool pause;
        RuleGenerator gen;

        void set_rules_to_life();
    public:
        Board();
        ~Board();
        void init_board();
        void init_quadrants();
        void init_center_dot();
        void init_gliders();

        void make_glider(int x, int y, int orientation);
        void clear_board();

        void update_board();
        int get_num_alive_neighbors(int x, int y);

        void update_colors();

        int* get_board();

        void randomize_rules();
        void set_density(int new_density);
        void modify_gliders(int factor);
        void toggle_changing_background();
        void toggle_pause();

        void rules_pretty();
        void rules_not_pretty();
};


#endif // BOARD_H_INCLUDED
