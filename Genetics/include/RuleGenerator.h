#ifndef RULEGENERATOR_H
#define RULEGENERATOR_H

#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <iostream>

class RuleGenerator
{
    public:
        RuleGenerator();
        virtual ~RuleGenerator();
        int* generate_one_mean();
        void add_seed(int* seed);
        void print_seeds();
    protected:
    private:
        std::vector<int*> seeds;
        void print_array(int *to_print);
        double random_double();
};

#endif // RULEGENERATOR_H
