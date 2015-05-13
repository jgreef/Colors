
#include "board.h"

Board::Board() : gen(8), e2(time(NULL)), dist(0, 1) {
    board = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
    board_buffer = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
    board_float = (double*)malloc(sizeof(double) * CELL_WIDTH * CELL_HEIGHT);
    board_buffer_float = (double*)malloc(sizeof(double) * CELL_WIDTH * CELL_HEIGHT);
    born = (double*)malloc(sizeof(double) * 18);
    stay_alive = (born+9);

    r_i = R_I;
    r_a = R_A;
    alpha_n = ALPHA_N;
    alpha_m = ALPHA_M;
    b1 = B1;
    b2 = B2;
    d1 = D1;
    d2 = D2;

    density = 50;
    num_gliders = 4;
    changing_background = true;
    pause = false;
    set_rules_to_life();
    update_algorithm = 0;


}


Board::~Board() {
    free(board);
    free(board_buffer);
    free(born);
    free(stay_alive);
}

//randomly initializes the board with density percent alive
//cells
void Board::init_board() {
    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            board[j*CELL_WIDTH+i] = (rand()%100 < density ? 1 : -1);
        }
    }
}

//puts num_gliders/4 dots of side length density/10 in each of the
//four quadrants. There is vertical and horizontal symmetry
void Board::init_quadrants() {
    clear_board();
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

//clears the board and draws a dot in the center with side length density/10
void Board::init_center_dot() {
    clear_board();
    int s = density/10;
    for(int i = (CELL_WIDTH - s)/2; i < (CELL_WIDTH + s)/2; i++ ) {
        for(int j = (CELL_HEIGHT - s)/2; j < (CELL_HEIGHT + s)/2; j++ ) {
            board[j*CELL_WIDTH+i] = 1;
        }
    }
}

void Board::init_smooth_life() {
    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            board_float[j*CELL_WIDTH+i] = (rand()%100 < density ? 1 : 0);
            board[j*CELL_WIDTH+i] = 0;
        }
    }
}

//this function clears the board then makes num_glider gliders in random
//locations and orientations. Makes sure to keep them away from the edge so
//we don't get seg faults
void Board::init_gliders() {
    clear_board();
    for(int i = 0; i < num_gliders; i++) {
        make_glider(rand()%(CELL_WIDTH-10)+5, rand()%(CELL_HEIGHT-10)+5, rand()%4);
    }
}

void Board::init_square_shell() {
    clear_board();
    int center_x = CELL_WIDTH/2;
    int center_y = CELL_HEIGHT/2;
    int side_length = rand() %(CELL_HEIGHT/2 -1) + 1;
    for(int i = center_x - side_length; i < center_x + side_length; i++) {
        for(int j = center_y - side_length; j < center_y + side_length; j++) {
            board[j*CELL_WIDTH+i] = 1;

        }
    }
    side_length--;
    for(int i = center_x - side_length; i < center_x + side_length; i++) {
        for(int j = center_y - side_length; j < center_y + side_length; j++) {
            board[j*CELL_WIDTH+i] = changing_background ? -1:0;

        }
    }
}

void Board::init_circle_shell() {
    clear_board();
    int r = rand() % (CELL_HEIGHT/2 -1) + 1;
    int center_x = CELL_WIDTH/2;
    int center_y = CELL_HEIGHT/2;
    int* circle_coords = (int*)malloc(sizeof(int) * r * r * 4 * 2);
    get_circle(center_x, center_y, r, circle_coords);
    int index = 0;
    int i, j;
    while(circle_coords[index] != -1) {
        i = circle_coords[index];
        index++;
        j = circle_coords[index];
        index++;

        board[j*CELL_WIDTH+i] = 1;
    }
    r--;
    index = 0;
    get_circle(center_x, center_y, r, circle_coords);
    while(circle_coords[index] != -1) {
        i = circle_coords[index];
        index++;
        j = circle_coords[index];
        index++;

        board[j*CELL_WIDTH+i] = changing_background ? -1:0;
    }
    free(circle_coords);
}

void Board::init_triangle_shell() {
    clear_board();
    int mag = 20 + rand() % 50;// rand() % (CELL_HEIGHT/8 -1) + 1;
    int* coords = (int*)malloc(2* sizeof(int) *CELL_HEIGHT * CELL_WIDTH);
    get_polygon(CELL_WIDTH/2, CELL_HEIGHT/2, mag, num_gliders, 0, coords);
    int index = 0;
    int i, j;
    while(coords[index] != -1) {
        i = coords[index];
        index++;
        j = coords[index];
        index++;

        board[j*CELL_WIDTH+i] = 1;
    }
    return;



}

//this function creates a glider on board at x,y in one of 4 orientations.
//it just manually sets each required alive cell. Assumes that all cells are
//dead in the required area. Doesn't account for the edge of the board
void Board::make_glider(int x, int y, int orientation) {
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

void Board::init_circle() {
    int r = rand()%30+3;
    int* circle_coords = (int*)malloc(sizeof(int) * r * r * 4 * 2);
    get_circle(rand()%CELL_WIDTH, rand()%CELL_HEIGHT-10, r, circle_coords);
    int index = 0;
    int i, j;
    while(circle_coords[index] != -1) {
        i = circle_coords[index];
        index++;
        j = circle_coords[index];
        index++;

        board[j*CELL_WIDTH+i] = 1;
    }
    free(circle_coords);
}

void Board::init_triangle() {
    int mag = rand()%30+10;
    int* coords = (int*)malloc(2* sizeof(int) *CELL_HEIGHT * CELL_WIDTH);
    get_mathy_triangle2(CELL_WIDTH/2, CELL_HEIGHT/2, mag, coords);
    int index = 0;
    int i, j;
    while(coords[index] != -1) {
        i = coords[index];
        index++;
        j = coords[index];
        index++;

        board[j*CELL_WIDTH+i] = 1;
    }
    free(coords);
}


//clears the board. If changing_background is true sets everything to -1
//so it will age, otherwise sets it to 0 so it won't
void Board::clear_board() {
    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            board[j*CELL_WIDTH+i]= changing_background ? -1:0 ;
        }
    }
}


//this function updates the board according to the ruleset
void Board::update_board() {
    switch(update_algorithm) {
        case 0:
            update_board_normal();
            break;
        case 1:
            update_board_smooth();
            break;
    }

    //free our old board
    free(board);
    //put the buffer into the spotlight
    board = board_buffer;
    //and get a new buffer
    board_buffer = (int*)malloc(sizeof(int) * CELL_WIDTH * CELL_HEIGHT);
}

void Board::update_board_smooth() {
    double n, m, r, rr;
    int *points = (int*) malloc(sizeof(int) * r_a * r_a * 4 * 2);
    int x, y, test_x, test_y, num_m, num_n;
    double r_a_2 = r_a*r_a;
    double r_i_2 = r_i*r_i;
    //squares (m)inus b
    double r_a_2_m_b = (r_a-0.5)*(r_a-0.5);
    double r_i_2_m_b = (r_i-0.5)*(r_i-0.5);
    //squares (p)lus b
    double r_a_2_p_b = (r_a+0.5)*(r_a+0.5);
    double r_i_2_p_b = (r_i+0.5)*(r_i+0.5);
    //iterate over every cell
    for(int j = 0; j < CELL_HEIGHT; j++) {
        for(int i = 0; i < CELL_WIDTH; i++) {
            /*get_circle(i, j, r_i, points);
            x = 0;
            y = 1;
            n = 0;
            num_n = 0;
            while(points[x] != -1) {
                n += board_float[points[y]*CELL_WIDTH+points[x]];
                num_n++;
                x += 2;
                y += 2;
            }
            get_circle(i, j, r_a, points);
            x = 0;
            y = 1;
            m = 0;
            num_m = 0;
            while(points[x] != -1) {
                m += board_float[points[y]*CELL_WIDTH+points[x]];
                num_m++;
                x += 2;
                y += 2;
            }*/
            n = 0;
            m = 0;
            num_n = 0;
            num_m = 0;
            for(y = -r_a; y <= r_a; y++) {
                test_y = j + y;
                if(test_y < 0)
                    test_y += CELL_HEIGHT;
                else if(test_y >= CELL_HEIGHT)
                    test_y -= CELL_HEIGHT;

                for(x = -r_a; x <= r_a; x++) {
                    test_x = i + x;
                    if(test_x < 0)
                        test_x += CELL_WIDTH;
                    else if(test_x >= CELL_WIDTH)
                        test_x -= CELL_WIDTH;

                //int yspan = r*sin(acos(-i/r));
                //for(int j = -yspan; j <= yspan; j++) {

                    rr = x*x + y*y;
                    r = sqrt(rr);
                    if(rr <= r_i_2_m_b) {
                        n += board_float[test_y*CELL_WIDTH+test_x];
                    }
                    else if(rr <= r_i_2_p_b) {
                        n += (r_i+0.5-r)*board_float[test_y*CELL_WIDTH+test_x];
                    }

                    if(rr <= r_a_2_m_b && rr >= r_i_2_p_b) {
                        m += board_float[test_y*CELL_WIDTH+test_x];
                    }
                    else if(rr <= r_a_2_p_b && rr >= r_a_2_m_b) {
                        m += (r_a+0.5-r)*board_float[test_y*CELL_WIDTH+test_x];
                    }
                    else if(rr <= r_i_2_p_b && rr >= r_i_2_m_b) {
                        m += (r_a+0.5-r)*board_float[test_y*CELL_WIDTH+test_x];
                    }

                }
            }

            //std::cout << "n " << n << std::endl;
            //std::cout << "m " << m << std::endl;
            //m = m - n;
            //num_m = num_m - num_n;
            n = n / (r_i_2 * PI);
            m = m / (PI * (r_a_2 - r_i_2));
            //n = n / num_n;
            //m = m / num_m;

            double new_val = s(n, m);
            board_buffer_float[j*CELL_WIDTH+i] = new_val;
            if(new_val < 0.01 && board_float[j*CELL_WIDTH+i] < 0.01) {
                board_buffer[j*CELL_WIDTH + i]--;
            }
            else if (new_val >= 0.01 && board_float[j*CELL_WIDTH+i] >= 0.01) {
                board_buffer[j*CELL_WIDTH + i]++;
            }
            else {
                board_buffer[j*CELL_WIDTH + i] = 0;
            }
        }
    }
    free(points);
    //free our old board
    free(board_float);
    //put the buffer into the spotlight
    board_float = board_buffer_float;
    //and get a new buffer
    board_buffer_float = (double*)malloc(sizeof(double) * CELL_WIDTH * CELL_HEIGHT);

}

double Board::s1(double x, double a, double alpha) {
    double to_return = 1/(1 + exp(((a-x) * 4)/alpha));
    return to_return;
}
double Board::s2(double x, double a, double b) {
    double to_return = s1(x, a, alpha_n) * (1 - s1(x, b, alpha_n));
    return to_return;
}
double Board::sm(double x, double y, double m) {
    double to_return = x * (1 - s1(m, 0.5, alpha_m)) + y * s1(m, 0.5, alpha_m);
    return to_return;
}
double Board::s(double n, double m) {
    double to_return = s2(n, sm(b1, d1, m), sm(b2, d2, m));
    return to_return;
}



void Board::update_board_normal() {
    //if we're paused, just update the colors
    if(pause) {
        update_colors();
        return;
    }

    //if a cell is positive it's alive, if it's negative or 0 then it's dead.
    //the actual value is how many generations it has been alive.
    //if the value is 0 then that dead cell is not supposed to age
    //this is how the non-changing background is implemented

    //iterate over every cell
    for(int j = 0; j < CELL_HEIGHT; j++) {
        for(int i = 0; i < CELL_WIDTH; i++) {
            //get how many alive neighbors it has
            int neighbors = get_num_alive_neighbors(i, j);

            //if dead
            if(board[j*CELL_WIDTH+i] <= 0) {
                //if supposed to be born
                if(born[neighbors] >= dist(e2))
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
                if(stay_alive[neighbors] >= dist(e2))
                    //then age it
                    board_buffer[j*CELL_WIDTH+i] = board[j*CELL_WIDTH+i] + 1;
                else
                    //otherwise kill it
                    board_buffer[j*CELL_WIDTH+i] = -1;

            }
        }
    }


}

//this gets the number of alive neighbors, wraps around edges so our real shape
//is a donut
int Board::get_num_alive_neighbors(int x, int y) {
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



//this function just updates the colors without updating the board
void Board::update_colors() {
    //iterate over the whole board
    for(int j = 0; j < CELL_HEIGHT; j++) {
        for(int i = 0; i < CELL_WIDTH; i++) {
            //increase age if alive
            if(board[j*CELL_WIDTH+i] > 0)
                board_buffer[j*CELL_WIDTH+i] = board[j*CELL_WIDTH+i] + 1;
            //and increase age if dead and supposed to change
            else if(board[j*CELL_WIDTH+i] < 0)
                board_buffer[j*CELL_WIDTH+i] = board[j*CELL_WIDTH+i] - 1;
        }
    }
}


void Board::set_rules_to_life() {
    //initial rules for the automata is game of life
    //int life_born[9] =       {0,1,1,1,1,0,0,1,0};
    //int life_stay_alive[9] = {0,1,1,0,0,1,1,1,1};
    //copier
    //int life_born[9] =       {0,1,0,1,0,1,0,1,0};
    //int life_stay_alive[9] = {0,1,0,1,0,1,0,1,0};
    //vote
    int life_born[9] =       {0,0,0,0,1,0,1,1,1};
    int life_stay_alive[9] = {0,0,0,1,0,1,1,1,1};
    //float life_born[9] =       {-.0224982,-0.0439246,0.16508,-0.381114,0.0156361,0.254988,0.354988,0.2501,0.31466};
    //float life_stay_alive[9] = {1,1,1,1,1,1,1,1,1};

    free(born);

    born = (double*)malloc(sizeof(double) * 18);
    stay_alive = (born + 9);


    for(int i = 0; i < 9; i++) {
        born[i] = life_born[i];
        stay_alive[i] = life_stay_alive[i];
    }

}

// 0 1 1 1 1 0 0 1 0
// 1 1 1 0 0 1 1 1 1c
int* Board::get_board() {
    return board;
}

double* Board::get_board_float() {
    return board_float;
}

void Board::randomize_rules() {
    for(int i = 0; i < 9; i++) {
        born[i] = (rand()%100>20 ? 1 : 0);
        stay_alive[i] = (rand()%100>20 ? 1 : 0);
    }

}

void Board::randomize_rules_non_deterministic() {
    std::normal_distribution<> ndist(0.0, 0.3);
    for(int i = 0; i < 9; i++) {
        born[i] = (rand()%100>20 ? ndist(e2) : 0);
        stay_alive[i] = (rand()%100>10 ? 1 : 0);
    }
}

void Board::randomize_rules_smooth() {
    std::normal_distribution<> ndist(0.01, 1.0);
    r_a = (ndist(e2)+1) * 15;
    r_i = r_a / ((ndist(e2) + 0.5)*3);
    alpha_n = ndist(e2)/5;
    alpha_m = ndist(e2)/5;
    b1 = ndist(e2)/2 * 0.1;
    b2 = b1 + ndist(e2)/3;
    d1 = ndist(e2)/2 * 0.1;
    d2 = b1 + ndist(e2)/3;
}

void Board::set_density(int new_density) {
    density = new_density;
}

void Board::set_update_algorithm(int new_algorithm) {
    update_algorithm = new_algorithm;
}

void Board::modify_gliders(int factor) {
    num_gliders += factor;
    if(num_gliders < 2)
        num_gliders = 2;
}

void Board::toggle_changing_background() {
    changing_background = !changing_background;
}

void Board::toggle_pause() {
    pause = !pause;
}

void Board::rules_pretty() {
    gen.add_seed(born);
    born = gen.generate_one_mean();
    stay_alive = (born + 9);
    init_center_dot();
}

void Board::rules_not_pretty() {
    free(born);
    born = gen.generate_one_mean();
    stay_alive = (born + 9);
    init_center_dot();
}

void Board::rules_pretty_float() {
    gen.add_seed(born);
    born = gen.generate_one_mean_float();
    stay_alive = (born + 9);
    init_center_dot();
}

void Board::rules_not_pretty_float() {
    free(born);
    born = gen.generate_one_mean_float();
    stay_alive = (born + 9);
    init_center_dot();
}

void Board::get_mathy_triangle2(int center_x, int center_y, int mag, int* points) {
    int index = 0;
    std::uniform_real_distribution<> udist(-PI, PI);
    std::uniform_real_distribution<> udist2(0.45*PI, 0.65*PI);

    float theta = udist(e2);

    float v1[2] = {std::sin(theta), std::cos(theta)};

    theta = theta + udist2(e2);
    float v2[2] = {std::sin(theta), std::cos(theta)};


    float v3[2] = {-1*(v1[0]+v2[0]), -1*(v1[1]+v2[1])};

    float norm_factor = v3[0]*v3[0] + v3[1]*v3[1];
    v3[0] = v3[0] / norm_factor;
    v3[1] = v3[1] / norm_factor;


    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            float x = i-center_x;
            float y = j-center_y;

            bool inside = true;
            inside = inside && (x*v1[0] + y*v1[1]) < mag;
            inside = inside && (x*v2[0] + y*v2[1]) < mag;
            inside = inside && (x*v3[0] + y*v3[1]) < mag;
            if(inside) {
                points[index] = i;
                index++;
                points[index] = j;
                index++;
            }

        }
    }
    points[index] = -1;
}


void Board::get_polygon(int center_x, int center_y, int mag, int dim, float irreg, int* points) {
    int index = 0;


    std::uniform_real_distribution<> udist(-PI, PI);
    std::uniform_real_distribution<> udist2((1-irreg)* 2*PI/dim, (1+irreg)*2*PI/dim);


    float **v = (float**)malloc(sizeof(float*)*dim);

    float x_sum = 0, y_sum = 0;
    float theta = udist(e2);

    int k;
    for(k = 0; k < dim - 1; k++) {
        theta += udist2(e2);
        v[k] = (float*)malloc(sizeof(float) * 2);
        v[k][0] = std::cos(theta);
        v[k][1] = std::sin(theta);
        x_sum += v[k][0];
        y_sum += v[k][1];

    }
    v[k] = (float*)malloc(sizeof(float) * 2);
    v[k][0] = -1*x_sum;
    v[k][1] = -1*y_sum;

    float norm_factor = v[k][0]*v[k][0] + v[k][1]*v[k][1];
    v[k][0] = v[k][0] / norm_factor;
    v[k][1] = v[k][1] / norm_factor;




    for(int i = 0; i < CELL_WIDTH; i++) {
        for(int j = 0; j < CELL_HEIGHT; j++) {
            float x = i-center_x;
            float y = j-center_y;

            bool inside1 = true;
            bool inside2 = false;
            for(int k = 0; k < dim; k++) {
                inside1 = inside1 && (x*v[k][0] + y*v[k][1]) < mag;
                inside2 = inside2 || (x*v[k][0] + y*v[k][1]) > mag - 5;

            }
            if(inside1 && inside2) {
                points[index] = i;
                index++;
                points[index] = j;
                index++;
            }
            /*if(inside2) {
                points2[index] = i;
                index++;
                points2[index] = j;
                index++;
            }*/


        }
    }
    points[index] = -1;
}




void Board::get_circle(int x, int y, int r, int* points) {
    int index = 0;
    int test_x, test_y;
    int rsquared = r*r;
    for(int i = -r; i <= r; i++) {
        test_x = i + x;
        if(test_x < 0)
            test_x += CELL_WIDTH;
        else if(test_x >= CELL_WIDTH)
            test_x -= CELL_WIDTH;

        //int yspan = r*sin(acos(-i/r));
        //for(int j = -yspan; j <= yspan; j++) {
        for(int j = -r; j <= r; j++) {
            test_y = j + y;
            if(test_y < 0)
                test_y += CELL_HEIGHT;
            else if(test_y >= CELL_HEIGHT)
                test_y -= CELL_HEIGHT;


            if(i * i + j * j <= rsquared) {
                points[index] = test_x;
                index++;
                points[index] = test_y;
                index++;
            }

        }
    }
    points[index] = -1;
}

void Board::print_rules() {
    for(int i = 0; i < 9; i++) {
        std::cout << born[i] << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < 9; i++) {
        std::cout << stay_alive[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

}
