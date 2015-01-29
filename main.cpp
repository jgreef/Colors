//
//  main.m
//  Colors
//
//  Created by Christine Cheng on 7/20/14.
//  Copyright (c) 2014 Christine Cheng. All rights reserved.
//
#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>



int screen_height, screen_width, cell_height, cell_width;

//define the size of your screen
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

//the size of each cell
#define PIXELS_PER_CELL 2

//and how many cells are in it
#define CELL_HEIGHT SCREEN_HEIGHT/PIXELS_PER_CELL
#define CELL_WIDTH SCREEN_WIDTH/PIXELS_PER_CELL

//initial rules for the automata is game of life
int born[9] = {0,0,0,1,0,0,0,0,0};
int stay_alive[9] = {0,0,1,1,0,0,0,0,0};


bool draw_colors = true; //start in colored mode
bool changing_background = true; //with changing backgrounds
bool seizure = false; //no seizures
bool pause = false; //not paused
uint8_t color_speed_divisor = 1; //full speed color changes

// these variables change what color dead and alive cells start at.
// so start them both at black
uint8_t alive_offset = 0, dead_offset = 0;

//4 gliders is a good number to start with, can be adjusted in sim
int num_gliders = 4;
Uint32 colors[256] = {0x000000, 0x070000, 0x0F0000, 0x170000, 0x1F0000, 0x270000, 0x2F0000, 0x370000, 0x3F0000, 0x470000, 0x4F0000, 0x570000, 0x5F0000, 0x670000, 0x6F0000, 0x770000, 0x7F0000, 0x870000, 0x8F0000, 0x970000, 0x9F0000, 0xA70000, 0xAF0000, 0xB70000, 0xBF0000, 0xC70000, 0xCF0000, 0xD70000, 0xDF0000, 0xE70000, 0xEF0000, 0xF70000, 0xFF0000, 0xFF0007, 0xFF000F, 0xFF0017, 0xFF001F, 0xFF0027, 0xFF002F, 0xFF0037, 0xFF003F, 0xFF0047, 0xFF004F, 0xFF0057, 0xFF005F, 0xFF0067, 0xFF006F, 0xFF0077, 0xFF007F, 0xFF0087, 0xFF008F, 0xFF0097, 0xFF009F, 0xFF00A7, 0xFF00AF, 0xFF00B7, 0xFF00BF, 0xFF00C7, 0xFF00CF, 0xFF00D7, 0xFF00DF, 0xFF00E7, 0xFF00EF, 0xFF00F7, 0xFF00FF, 0xF700FF, 0xEF00FF, 0xE700FF, 0xDF00FF, 0xD700FF, 0xCF00FF, 0xC700FF, 0xBF00FF, 0xB700FF, 0xAF00FF, 0xA700FF, 0x9F00FF, 0x9700FF, 0x8F00FF, 0x8700FF, 0x7F00FF, 0x7700FF, 0x6F00FF, 0x6700FF, 0x5F00FF, 0x5700FF, 0x4F00FF, 0x4700FF, 0x3F00FF, 0x3700FF, 0x2F00FF, 0x2700FF, 0x1F00FF, 0x1700FF, 0x0F00FF, 0x0700FF, 0x0000FF, 0x0007FF, 0x000FFF, 0x0017FF, 0x001FFF, 0x0027FF, 0x002FFF, 0x0037FF, 0x003FFF, 0x0047FF, 0x004FFF, 0x0057FF, 0x005FFF, 0x0067FF, 0x006FFF, 0x0077FF, 0x007FFF, 0x0087FF, 0x008FFF, 0x0097FF, 0x009FFF, 0x00A7FF, 0x00AFFF, 0x00B7FF, 0x00BFFF, 0x00C7FF, 0x00CFFF, 0x00D7FF, 0x00DFFF, 0x00E7FF, 0x00EFFF, 0x00F7FF, 0x00FFFF, 0x00FFF7, 0x00FFEF, 0x00FFE7, 0x00FFDF, 0x00FFD7, 0x00FFCF, 0x00FFC7, 0x00FFBF, 0x00FFB7, 0x00FFAF, 0x00FFA7, 0x00FF9F, 0x00FF97, 0x00FF8F, 0x00FF87, 0x00FF7F, 0x00FF77, 0x00FF6F, 0x00FF67, 0x00FF5F, 0x00FF57, 0x00FF4F, 0x00FF47, 0x00FF3F, 0x00FF37, 0x00FF2F, 0x00FF27, 0x00FF1F, 0x00FF17, 0x00FF0F, 0x00FF07, 0x00FF00, 0x07FF00, 0x0FFF00, 0x17FF00, 0x1FFF00, 0x27FF00, 0x2FFF00, 0x37FF00, 0x3FFF00, 0x47FF00, 0x4FFF00, 0x57FF00, 0x5FFF00, 0x67FF00, 0x6FFF00, 0x77FF00, 0x7FFF00, 0x87FF00, 0x8FFF00, 0x97FF00, 0x9FFF00, 0xA7FF00, 0xAFFF00, 0xB7FF00, 0xBFFF00, 0xC7FF00, 0xCFFF00, 0xD7FF00, 0xDFFF00, 0xE7FF00, 0xEFFF00, 0xF7FF00, 0xFFFF00, 0xFFFB00, 0xFFF800, 0xFFF400, 0xFFF100, 0xFFED00, 0xFFEA00, 0xFFE700, 0xFFE300, 0xFFE000, 0xFFDC00, 0xFFD901, 0xFFD601, 0xFFD201, 0xFFCF01, 0xFFCB01, 0xFFC801, 0xFFC501, 0xFFC101, 0xFFBE01, 0xFFBA01, 0xFFB701, 0xFFB402, 0xFFB002, 0xFFAD02, 0xFFA902, 0xFFA602, 0xFFA302, 0xFF9F02, 0xFF9C02, 0xFF9802, 0xFF9502, 0xFF9203, 0xF68D02, 0xEE8802, 0xE68302, 0xDE7F02, 0xD57A02, 0xCD7502, 0xC57102, 0xBD6C02, 0xB46702, 0xAC6202, 0xA45E01, 0x9C5901, 0x945401, 0x8B5001, 0x834B01, 0x7B4601, 0x734101, 0x6A3D01, 0x623801, 0x5A3301, 0x522F00, 0x4A2A00, 0x412500, 0x392000, 0x311C00, 0x291700, 0x201200, 0x180E00, 0x100900, 0x080400};
int density = 50; //50% density of randomly generated cells

//this function sets the pixels at (x,y) on the board to color accounting for the fact
//that cells are not always 1x1 pixels
void set_pixel(Uint32* pixels, int x, int y, Uint32 color) {
    //just sets the correct block of pixels to the color
    for(int i = x*PIXELS_PER_CELL; i < x*PIXELS_PER_CELL+PIXELS_PER_CELL; i++) {
        for(int j = y*PIXELS_PER_CELL; j < y*PIXELS_PER_CELL+PIXELS_PER_CELL; j++) {
            pixels[j*SCREEN_WIDTH+i] = color;
        }
    }
}

//this function converts a board of cells into a screen's worth of pixels
void draw_board(Uint32* pixels, int* board) {
    //if we're drawing in color
    if(draw_colors) {
        //go over the entire board
        for(int j = 0; j < CELL_HEIGHT; j++) {
            for(int i = 0; i < CELL_WIDTH; i++) {
                //if we're alive modulo the age of the cell by the number of colors to get which color to draw and draw it
                if(board[j*CELL_WIDTH+i] > 0)
                    set_pixel(pixels, i, j, colors[((board[j*CELL_WIDTH+i]+alive_offset)/color_speed_divisor) % 256]);
                else //do the same if we're dead, but with negative age instead
                    set_pixel(pixels, i, j, colors[((-board[j*CELL_WIDTH+i]+dead_offset)/color_speed_divisor) % 256]);
            }
        }
    }
    //if we're doing black and white
    else {
        //iterate over the whole board
        for(int j = 0; j < CELL_HEIGHT; j++) {
            for(int i = 0; i < CELL_WIDTH; i++) {
                //and draw white if alive
                if(board[j*CELL_WIDTH+i] > 0)
                    set_pixel(pixels, i, j, 0xFFFFFF);
                //and black if dead
                else
                    set_pixel(pixels, i, j, 0x000000);
            }
        }
    }
}

//this function just updates the colors without updating the board
void update_colors(int* board) {
    //iterate over the whole board
    for(int j = 0; j < CELL_HEIGHT; j++) {
        for(int i = 0; i < CELL_WIDTH; i++) {
            //increase age if alive
            if(board[j*CELL_WIDTH+i] > 0)
                board[j*CELL_WIDTH+i]++;
            //and increase age if dead and supposed to change
            else if(board[j*CELL_WIDTH+i] < 0)
                board[j*CELL_WIDTH+i]--;
        }
    }
}

//this gets the number of alive neighbors, wraps around edges so our real shape
//is a donut
int get_num_alive_neighbors(int* board, int x, int y) {
    int count = 0;
    int check_x, check_y;
    //iterate over the 3x3 square around x,y
    for(int j = y-1; j <= y+1; j++) {
        for(int i = x-1; i <= x+1; i++) {

            check_x = i;
            check_y = j;

            //if we're on x,y skip it
            if(j==y && i==x)
                continue;
            //if we're past any edges, wrap around to the other side
            if(j < 0)
                check_y = CELL_HEIGHT-1;
            if(j >= CELL_HEIGHT)
                check_y = 0;
            if(i < 0)
                check_x = CELL_WIDTH-1;
            if(i >= CELL_WIDTH)
                check_x = 0;

            //and check the coordinate, if it's alive increase count
            if(board[check_y*CELL_WIDTH+check_x] > 0)
                count++;
        }
    }
    return count;
}


//this function updates the board according to the ruleset
void update_board(int* board, int* board_buffer) {
    //if a cell is positive it's alive, if it's negative or 0 then it's dead.
    //the actual value is how many generations it has been alive.
    //if the value is 0 then that dead cell is not supposed to age
    //this is how the non-changing background is implemented

    //iterate over every cell
    for(int j = 0; j < CELL_HEIGHT; j++) {
        for(int i = 0; i < CELL_WIDTH; i++) {
            //get how many alive neighbors it has
            int neighbors = get_num_alive_neighbors(board, i, j);

            //if dead
            if(board[j*CELL_WIDTH+i] <= 0) {
                //if supposed to be born
                if(born[neighbors] == 1)
                    // then make it alive with an age of 1
                    board_buffer[j*CELL_WIDTH+i] = 1;
                //if its still dead
                //and it's aging
                else if(board[j*CELL_WIDTH+i] < 0)
                    //then age it
                    board_buffer[j*CELL_WIDTH+i] = board[j*CELL_WIDTH+i] - 1;
                else
                    //keep it dead
                    board_buffer[j*CELL_WIDTH+i] = 0;
            }
            //if alive
            else {
                //and it's supposed to stay alive, and it's not older than the max age
                if(stay_alive[neighbors] && board[j*CELL_WIDTH+i] < 256 * color_speed_divisor)
                    //then age it
                    board_buffer[j*CELL_WIDTH+i] = board[j*CELL_WIDTH+i] + 1;
                else
                    //otherwise kill it
                    board_buffer[j*CELL_WIDTH+i] = -1;

            }
        }
    }

}


//clears the board. If changing_background is true sets everything to -1
//so it will age, otherwise sets it to 0 so it won't
void clear_board(int *board) {
    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            board[j*CELL_WIDTH+i]= changing_background ? -1:0 ;
        }
    }
}

//randomly initializes the board with density percent alive
//cells
void init_board(int *board) {
    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            board[j*CELL_WIDTH+i] = (rand()%100<density ? 1 : -1);
        }
    }
}

//randomly generates the color offsets and ruleset.
void init_params() {
    dead_offset = rand() % 256;
    alive_offset = rand() % 256;
    for(int i = 0; i < 9; i++) {
        born[i] = (rand()%100>20 ? 1 : 0);
        stay_alive[i] = (rand()%100>20 ? 1 : 0);
    }
}

//this function creates a glider on board at x,y in one of 4 orientations.
//it just manually sets each required alive cell. Assumes that all cells are
//dead in the required area. Doesn't account for the edge of the board
void make_glider(int *board, int x, int y, int orientation) {
    if(orientation == 0) {
        board[(y+0)*CELL_WIDTH+(x+1)]=1;
        board[(y+1)*CELL_WIDTH+(x+2)]=1;
        board[(y+2)*CELL_WIDTH+(x+0)]=1;
        board[(y+2)*CELL_WIDTH+(x+1)]=1;
        board[(y+2)*CELL_WIDTH+(x+2)]=1;
    }
    else if(orientation == 1) {
        board[(y+0)*CELL_WIDTH+(x+1)]=1;
        board[(y+0)*CELL_WIDTH+(x+2)]=1;
        board[(y+1)*CELL_WIDTH+(x+0)]=1;
        board[(y+1)*CELL_WIDTH+(x+2)]=1;
        board[(y+2)*CELL_WIDTH+(x+2)]=1;
    }
    else if(orientation == 2) {
        board[(y+0)*CELL_WIDTH+(x+1)]=1;
        board[(y+0)*CELL_WIDTH+(x+2)]=1;
        board[(y+0)*CELL_WIDTH+(x+0)]=1;
        board[(y+1)*CELL_WIDTH+(x+0)]=1;
        board[(y+2)*CELL_WIDTH+(x+1)]=1;
    }
    else if(orientation == 3) {
        board[(y+0)*CELL_WIDTH+(x+0)]=1;
        board[(y+1)*CELL_WIDTH+(x+0)]=1;
        board[(y+1)*CELL_WIDTH+(x+2)]=1;
        board[(y+2)*CELL_WIDTH+(x+0)]=1;
        board[(y+2)*CELL_WIDTH+(x+1)]=1;
    }
}

//this function clears the board then makes num_glider gliders in random
//locations and orientations. Makes sure to keep them away from the edge so
//we don't get seg faults
void init_gliders(int *board) {
    clear_board(board);
    for(int i = 0; i < num_gliders; i++) {
        make_glider(board, rand()%(CELL_WIDTH-10)+5, rand()%(CELL_HEIGHT-10)+5, rand()%4);
    }
}

//clears the board and draws a dot in the center with side length density/10
void init_center_dot(int *board) {
    clear_board(board);
    int s = density/10;
    for(int i = (CELL_WIDTH - s)/2; i < (CELL_WIDTH + s)/2; i++ ) {
        for(int j = (CELL_HEIGHT - s)/2; j < (CELL_HEIGHT + s)/2; j++ ) {
            board[j*CELL_WIDTH+i] = 1;
        }
    }
}

//puts num_gliders/4 dots of side length density/10 in each of the
//four quadrants. There is vertical and horizontal symmetry
void init_quadrants(int *board) {
    clear_board(board);
    int x, y;
    for(int i = 0; i < num_gliders / 4; i++) {
        x = rand() % (CELL_WIDTH/2 - density/10);
        y = rand() % (CELL_HEIGHT/2 - density/10);

        //u++ <=> u = u + 1
        for(int u = 0; u < density/10; u++) {
            for(int v = 0; v < density/10; v++) {
                //top left
                board[(y+u)*CELL_WIDTH+(x+v)] = 1;
                //top right quadrant
                board[(y+u)*CELL_WIDTH+(CELL_WIDTH-x-v)] = 1;
                //bottom left
                board[(CELL_HEIGHT-y-u)*CELL_WIDTH+(x+v)] = 1;
                //bottom right
                board[(CELL_HEIGHT-y-u)*CELL_WIDTH+(CELL_WIDTH-x-v)] = 1;
            }
        }
    }
}

int main(int argc, char * arg[])
{

    SDL_Surface* screen = NULL;
    Uint32 *pixels;
    int* board = NULL, *board_buffer = NULL;
    SDL_Event events;

    board = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
    board_buffer = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
    srand (time(NULL));


    init_board(board);

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init" << std::endl;

        return -1;
    }

    // create a window
    SDL_Window * window = SDL_CreateWindow(
                                           "SDL 2 window",             // window title
                                           SDL_WINDOWPOS_CENTERED,     // x position, centered
                                           SDL_WINDOWPOS_CENTERED,     // y position, centered
                                           SCREEN_WIDTH,               // width, in pixels
                                           SCREEN_HEIGHT,              // height, in pixels
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN          // flags
                                           );
    SDL_GetWindowSize(window, &screen_width, &screen_height);


    screen = SDL_GetWindowSurface(window);
    pixels = (Uint32*)screen->pixels;
    bool running = true;
    while(running) {
        while(SDL_PollEvent(&events)) {
            switch(events.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(events.key.keysym.sym) {
                        case SDLK_q:
                            running = false;
                            break;
                        //increases the number of gliders
                        case SDLK_UP:
                            num_gliders++;
                            break;
                        //decreases the number of gliders to generate
                        case SDLK_DOWN:
                            num_gliders--;
                            break;
                        //creates num_gliders gliders in random positions on the board
                        case SDLK_g:
                            init_gliders(board);
                            break;
                        //randomizes the whole board
                        case SDLK_SPACE:
                            init_board(board);
                            break;
                        //randomizes the cellular automata ruleset
                        case SDLK_d:
                            init_params();
                            break;
                        //draws num_gliders/4 dots of side length density/10 symmetrically in each quadrant
                        case SDLK_e:
                            init_quadrants(board);
                            break;
                        //toggles between black and white and colors
                        case SDLK_c:
                            draw_colors = !draw_colors;
                            break;
                        //makes a center square of side length density/10
                        case SDLK_a:
                            init_center_dot(board);
                            break;
                        //toggles between having the background
                        case SDLK_v:
                            changing_background = !changing_background;
                            break;
                        // prints the current board to the terminal, useful for debugging
                        case SDLK_p:
                            for(int j = 0; j < CELL_HEIGHT; j++) {
                                for(int i = 0; i < CELL_WIDTH; i++) {
                                    std::cout << board[j*CELL_WIDTH+i] << ", ";
                                }
                                std::cout << std::endl;
                            }
                            break;
                        // toggles seizure mode, which toggles rapidly between color and b/w mode
                        case SDLK_s:
                            seizure = !seizure;
                            break;
                        // this changes the density of live cells that random scenes
                        // are generated with. The integer is the percent alive cells
                        case SDLK_0:
                            density = 1;
                            break;
                        case SDLK_1:
                            density = 10;
                            break;
                        case SDLK_2:
                            density = 20;
                            break;
                        case SDLK_3:
                            density = 30;
                            break;
                        case SDLK_4:
                            density = 40;
                            break;
                        case SDLK_5:
                            density = 50;
                            break;
                        case SDLK_6:
                            density = 60;
                            break;
                        case SDLK_7:
                            density = 70;
                            break;
                        case SDLK_8:
                            density = 80;
                            break;
                        case SDLK_9:
                            density = 90;
                            break;
                        case SDLK_LSHIFT:
                            pause = !pause;
                            break;
                        // these change how fast the colors cycle, higher divisor means slower
                        case SDLK_F1:
                            color_speed_divisor = 1;
                            break;
                        case SDLK_F2:
                            color_speed_divisor = 2;
                            break;
                        case SDLK_F3:
                            color_speed_divisor = 4;
                            break;
                        case SDLK_F4:
                            color_speed_divisor = 7;
                            break;
                        case SDLK_F5:
                            color_speed_divisor = 11;
                            break;
                        case SDLK_F6:
                            color_speed_divisor = 16;
                            break;
                        case SDLK_F7:
                            color_speed_divisor = 22;
                            break;
                        case SDLK_F8:
                            color_speed_divisor = 29;
                            break;
                        case SDLK_F9:
                            color_speed_divisor = 37;
                            break;
                        case SDLK_F10:
                            color_speed_divisor = 46;
                            break;
                        case SDLK_F11:
                            color_speed_divisor = 56;
                            break;
                        case SDLK_F12:
                            color_speed_divisor = 67;
                            break;

                    }

            }
        }

        //translate board to pixels
        draw_board(pixels, board);
        //and draw it
        SDL_UpdateWindowSurface(window);

        //if we're seizing then toggle drawing colors
        if(seizure)
            draw_colors = !draw_colors;


        //if we're pausing the simulation
        if(pause) {
            //we still want to update colors on the board
            update_colors(board);
            //but continue over updating the board
            continue;
        }
        else {
            update_board(board, board_buffer);
            //free our old board
            free(board);
            //put the buffer into the spotlight
            board = board_buffer;
            //and get a new buffer
            board_buffer = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
        }


    }


    // clean up
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
