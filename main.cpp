#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <getopt.h>

#include "solvers.hpp"
#include "launching.hpp"

#include "knapsack.hpp"
#include "tsp.hpp"



const char* SHORT_OPTS = "b:d:i:n:s:";

typedef std::function<void(std::istream&, std::ostream&, double, double, int, int, int)> Launcher;

const std::map<std::pair<std::string, std::string>, Launcher> launchers {
    {{"knapsack", "local"}, launch<knapsack::Task, Solver_local<knapsack::Task>>},
    {{"knapsack", "bnb"}, launch<knapsack::Task, Solver_bnb<knapsack::Task>>},
    {{"tsp", "local"}, launch<tsp::Task, Solver_local<tsp::Task>>},
};



void print_usage_str(std::ostream& os, const char* main_name) {
    os << "Usage: " << main_name << " <task_name> <solver_name> [-b <best_score>] [-d <detail_level>] [-i <max_iterations>] [-n <number_of_launches>] [-s <step>]" << std::endl;
}
void print_available_task_solver_combinations(std::ostream& os) {
    os << "Available task and solver combinations: [";
    for (decltype(launchers)::const_iterator it = launchers.begin(); it != launchers.end(); ++it) {
        if (it != launchers.begin()) {
            os << " | ";
        }
        os << (*it).first.first << " " << (*it).first.second;
    }
    os << "]" << std::endl;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected task name" << std::endl;
        print_usage_str(std::cerr, argv[0]);
        return -1;
    }
    if (argc < 3) {
        std::cerr << "Expected solver name" << std::endl;
        print_usage_str(std::cerr, argv[0]);
        return -1;
    }
    
    std::string task_name = argv[1];
    std::string solver_name = argv[2];
    decltype(launchers)::const_iterator it = launchers.find({task_name, solver_name});
    
    if (it == launchers.end()) {
        std::cerr << "task and solver combination \"" << task_name << " " << solver_name << "\" is not supported" << std::endl;
        print_available_task_solver_combinations(std::cerr);
        return -1;
    }
    
    const Launcher launcher = (*it).second;
    
    
    
    double best_score = -INFINITY;
    int detail_level = 0;
    int max_iterations = 100;
    int number_of_launches = 1;
    double step = 2;
    
    int opt;
    optind = 2;
    
    while ((opt = getopt(argc, argv, SHORT_OPTS)) != -1) {
        switch(opt) {
        case 'b':
            best_score = atof(optarg);
            break;
        case 'd':
            detail_level = atoi(optarg);
            break;
        case 'i':
            max_iterations = atoi(optarg);
            break;
        case 'n':
            number_of_launches = atoi(optarg);
            break;
        case 's':
            step = atof(optarg);
            break;
        default:
            std::cerr << "Unknown option \"" << optopt << "\"" << std::endl;
            print_usage_str(std::cerr, argv[0]);
            return -1;
        }
    }
    
    
    
    launcher(std::cin, std::cout, step, best_score, max_iterations, number_of_launches, detail_level);
    
    
    
    return 0;
}