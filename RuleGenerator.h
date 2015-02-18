#ifndef RULEGENERATOR_H
#define RULEGENERATOR_H

#include <algorithm>    // std::random_shuffle
#include <deque>       // std::deque
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <iostream>

class RuleGenerator
{
    public:
        RuleGenerator(unsigned int new_max_seeds);
        virtual ~RuleGenerator();
        double* generate_one_mean();
        double* generate_one_mean_float();
        void add_seed(double* seed);
        void print_seeds();
        void print_array(double *to_print);
    protected:
    private:
        unsigned int max_seeds;
        std::deque<double*> seeds;
        double random_double();
};

#endif // RULEGENERATOR_H
