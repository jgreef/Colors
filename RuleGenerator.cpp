#include "RuleGenerator.h"



RuleGenerator::RuleGenerator(unsigned int new_max_seeds)
{
    max_seeds = new_max_seeds;
}

RuleGenerator::~RuleGenerator()
{
    //dtor
}

double* RuleGenerator::generate_one_mean() {
    double probabilities[18];
    double *to_return = (double*) malloc(sizeof(double) * 18);
    for(int i = 0; i < 18; i++) {
        probabilities[i] = 0;
    }

    for(std::deque<double*>::iterator it = seeds.begin(); it != seeds.end(); it++) {
        for(int k = 0; k < 18; k++) {
            probabilities[k] += (*it)[k];
        }
    }

    for(int i = 0; i < 18; i++) {
        probabilities[i] = probabilities[i] / seeds.size();
    }

    for(int i = 0; i < 18; i++) {
        to_return[i] = probabilities[i] >= (random_double() - 0.1) ? 1 : 0;
    }

    return to_return;
}

void RuleGenerator::add_seed(double* seed) {
    seeds.push_back(seed);
    if(seeds.size() > max_seeds) {
        free(seeds.front());
        seeds.pop_front();
    }
}

void RuleGenerator::print_seeds() {
    for(std::deque<double*>::iterator it = seeds.begin(); it != seeds.end(); it++) {
        print_array(*it);
    }
}

void RuleGenerator::print_array(double *to_print) {
    std::cout << "[ ";
    for(int i = 0; i < 18; i++) {
        std::cout << to_print[i] << " ";
    }
    std::cout << "]" << std::endl;
    return;
}

double RuleGenerator::random_double() {
    double f = (double)rand() / RAND_MAX;
    return f;
}
