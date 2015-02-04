#include <iostream>
#include <fstream>
#include <string>
#include "RuleGenerator.h"

using namespace std;

void print_array(int *to_print) {
    cout << "[ ";
    for(int i = 0; i < 18; i++) {
        cout << to_print[i] << " ";
    }
    cout << "]" << endl;
    return;
}

void load_file(string filename, RuleGenerator *gen) {
    ifstream seed_file;
    seed_file.open(filename.c_str());
    string line;
    if (seed_file.is_open()) {
        while (getline(seed_file, line)) {
            int *next_seed = (int*) malloc(sizeof(int) * 18);
            for(std::string::size_type i = 0; i < line.size(); ++i) {
                next_seed[i] = (line[i] == '1') ? 1 : 0;
            }
            print_array(next_seed);
            gen->add_seed(next_seed);
        }
        seed_file.close();
    }

}

int main()
{
    RuleGenerator gen;
    srand (time(NULL));

    string seed_filename;
    cout << "type filename" << endl;
    cin >> seed_filename;
    if(seed_filename == "a")
        seed_filename = "test_seeds1.txt";
    load_file(seed_filename, &gen);

    cout << "loaded seeds are: " << endl;
    gen.print_seeds();

    cout << "generated seeds are: " << endl;
    for(int i = 0; i < 20; i++) {
        int *test = gen.generate_one_mean();
        print_array(test);
        free(test);
    }

    return 0;
}
