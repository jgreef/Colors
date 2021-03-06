//
//  main.m
//  Colors
//
//
#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include "board.h"
#include "screen.h"
#include "RuleGenerator.h"


//this function checks if there is user input and reacts accordingly
//returns whether the program should keep running,
//false if it should quit, true if it should continue
bool do_user_input(Board *board, Screen *screen) {
    static SDL_Event events;
    while(SDL_PollEvent(&events)) {
        switch(events.type) {
            case SDL_QUIT:
                return false;
                break;
            case SDL_KEYDOWN:
                switch(events.key.keysym.sym) {
                    case SDLK_q:
                        return false;
                        break;
                    //increases the number of gliders by 1
                    case SDLK_UP:
                        board->modify_gliders(1);
                        break;
                    //decreases the number of gliders to generate by 1
                    case SDLK_DOWN:
                        board->modify_gliders(-1);
                        break;

                    //increases the number of gliders by 4
                    case SDLK_RIGHT:
                        board->modify_gliders(4);
                        break;
                    //decreases the number of gliders to generate by 4
                    case SDLK_LEFT:
                        board->modify_gliders(-4);
                        break;
                    //creates num_gliders gliders in random positions on the board
                    case SDLK_g:
                        board->init_gliders();
                        break;
                    //randomizes the whole board
                    case SDLK_SPACE:
                        board->init_board();
                        break;
                    //clears the board
                    case SDLK_z:
                        board->clear_board();
                        break;
                    //adds a circle to the board
                    case SDLK_o:
                        board->init_circle();
                        break;
                    //randomizes the cellular automata ruleset
                    case SDLK_d:
                        board->randomize_rules();
                        screen->reset_colors();
                        break;
                    case SDLK_j:
                        board->randomize_rules_smooth();
                        board->init_smooth_life();
                        break;
                    //randomizes the cellular automata ruleset with nondeterministic behavior
                    case SDLK_r:
                        board->randomize_rules_non_deterministic();
                        screen->reset_colors();
                        break;
                    //changes rules to life
                    case SDLK_l:
                        board->set_rules_to_life();
                        screen->reset_colors();
                        break;
                    //draws num_gliders/4 dots of side length density/10 symmetrically in each quadrant
                    case SDLK_w:
                        board->init_circle_shell();
                        break;
                    case SDLK_t:
                        board->init_triangle_shell();
                        break;
                    case SDLK_e:
                        board->init_quadrants();
                        break;
                    //toggles between black and white and colors
                    case SDLK_c:
                        screen->flip_draw_colors();
                        break;
                    //randomize colors
                    case SDLK_x:
                        screen->reset_colors();
                        break;
                    //makes a center square of side length density/10
                    case SDLK_a:
                        board->init_center_dot();
                        break;
                    case SDLK_s:
                        board->init_square_shell();
                        break;
                    //toggles between having the background
                    case SDLK_v:
                        board->toggle_changing_background();
                        break;
                    case SDLK_k:
                        board->init_smooth_life();
                        break;
                    //set board to normal automata
                    case SDLK_n:
                        board->set_update_algorithm(0);
                        screen->flip_draw_smooth();
                        break;
                    //set board to smooth automata
                    case SDLK_m:
                        board->set_update_algorithm(1);
                        board->init_smooth_life();
                        screen->flip_draw_smooth();
                        break;
                    case SDLK_p:
                        board->print_rules();
                        break;

                    // this changes the density of live cells that random scenes
                    // are generated with-> The integer is the percent alive cells
                    case SDLK_0:
                        board->set_density(0);
                        break;
                    case SDLK_1:
                        board->set_density(10);
                        break;
                    case SDLK_2:
                        board->set_density(20);
                        break;
                    case SDLK_3:
                        board->set_density(30);
                        break;
                    case SDLK_4:
                        board->set_density(40);
                        break;
                    case SDLK_5:
                        board->set_density(50);
                        break;
                    case SDLK_6:
                        board->set_density(60);
                        break;
                    case SDLK_7:
                        board->set_density(70);
                        break;
                    case SDLK_8:
                        board->set_density(80);
                        break;
                    case SDLK_9:
                        board->set_density(90);
                        break;
                    case SDLK_LSHIFT:
                        board->toggle_pause();
                        break;
                    // these change how fast the colors cycle, higher divisor means slower
                    case SDLK_F1:
                        screen->set_color_speed_divisor(1);
                        break;
                    case SDLK_F2:
                        screen->set_color_speed_divisor(2);
                        break;
                    case SDLK_F3:
                        screen->set_color_speed_divisor(4);
                        break;
                    case SDLK_F4:
                        screen->set_color_speed_divisor(7);
                        break;
                    case SDLK_F5:
                        screen->set_color_speed_divisor(11);
                        break;
                    case SDLK_F6:
                        screen->set_color_speed_divisor(16);
                        break;
                    case SDLK_F7:
                        screen->set_color_speed_divisor(22);
                        break;
                    case SDLK_F8:
                        screen->set_color_speed_divisor(29);
                        break;
                    case SDLK_F9:
                        screen->set_color_speed_divisor(37);
                        break;
                    case SDLK_F10:
                        screen->set_color_speed_divisor(46);
                        break;
                    case SDLK_F11:
                        screen->set_color_speed_divisor(56);
                        break;
                    case SDLK_F12:
                        screen->set_color_speed_divisor(67);
                        break;
                    case SDLK_EQUALS:
                        board->rules_pretty();
                        screen->reset_colors();
                        break;
                    case SDLK_MINUS:
                        board->rules_not_pretty();
                        screen->reset_colors();
                        break;
                    case SDLK_LEFTBRACKET:
                        board->rules_not_pretty_float();
                        screen->reset_colors();
                        break;
                    case SDLK_RIGHTBRACKET:
                        board->rules_pretty_float();
                        screen->reset_colors();
                        break;

                }
        }
    }
    return true;
}


void teststuff() {
    RuleGenerator gen(20);

    double *seed = (double*) malloc(18*sizeof(double));
    for(int i = 0; i < 18; i++) {
        seed[i] = 1;
    }
    gen.print_array(seed);
    gen.add_seed(seed);
    seed = gen.generate_one_mean_float();
    gen.print_array(seed);
    return;
}

int main(int argc, char * arg[])
{
    srand (time(NULL));


    //teststuff();
    //return 0;
    Board board;
    //return 0;
    Screen screen(&board);

    board.init_board();
    board.set_update_algorithm(0);
    board.init_board();

    bool running = true;
    while(running) {
        //translate board to pixels
        screen.draw_board();
        //and draw it
        screen.update_window();

        board.update_board();

        running = do_user_input(&board, &screen);

    }

    //return (this deconstructs screen and board and thus cleans up everything)
    return 0;
}
