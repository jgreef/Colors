#include "RuleGenerator.h"



RuleGenerator::RuleGenerator()
{
}

RuleGenerator::~RuleGenerator()
{
    //dtor
}

int* RuleGenerator::generate_one_mean() {
    double probabilities[18];
    int *to_return = (int*) malloc(sizeof(int) * 18);
    for(int i = 0; i < 18; i++) {
        probabilities[i] = 0;
    }

    for(std::vector<int*>::size_type i = 0; i != seeds.size(); i++) {
        for(int k = 0; k < 18; k++) {
            probabilities[k] += seeds[i][k];
        }
    }

    for(int i = 0; i < 18; i++) {
        probabilities[i] = probabilities[i] / seeds.size();
    }

    for(int i = 0; i < 18; i++) {
        to_return[i] = probabilities[i] >= random_double() ? 1 : 0;
    }

    return to_return;
}

void RuleGenerator::add_seed(int* seed) {
    seeds.push_back(seed);
}

void RuleGenerator::print_seeds() {
    for(std::vector<int*>::size_type i = 0; i != seeds.size(); i++) {
        print_array(seeds[i]);
    }
}

void RuleGenerator::print_array(int *to_print) {
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
