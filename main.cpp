#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <getopt.h>

#include "solvers.hpp"
#include "launching.hpp"

#include "knapsack.hpp"
#include "tsp.hpp"



const char* SHORT_OPTS = "d:i:n:";

const std::map<std::string, std::function<void(int, int, int)>> launchers {
    {"knapsack", launch<knapsack::Task, Solver_local<knapsack::Task>>},
    {"tsp", launch<tsp::Task, Solver_local<tsp::Task>>},
};



void print_usage_str(std::ostream& os, const char* main_name) {
    os << "Usage: " << main_name << " <task_name> [-d <detail_level>] [-i <max_iterations>] [-n <number_of_launches>]" << std::endl;
}
void print_available_tasks(std::ostream& os) {
    os << "Available tasks: [";
    for (decltype(launchers)::const_iterator it = launchers.begin(); it != launchers.end(); ++it) {
        if (it != launchers.begin()) {
            os << " | ";
        }
        os << (*it).first;
    }
    os << "]" << std::endl;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected task name" << std::endl;
        print_usage_str(std::cerr, argv[0]);
        return -1;
    }
    
    std::string task_name = argv[1];
    decltype(launchers)::const_iterator it = launchers.find(task_name);
    
    if (it == launchers.end()) {
        std::cerr << "No task with name \"" << task_name << "\"" << std::endl;
        print_available_tasks(std::cerr);
        return -1;
    }
    
    const std::function<void(int, int, int)> launcher = (*it).second;
    
    
    
    int detail_level = 0;
    int max_iterations = 100;
    int number_of_launches = 1;
    
    int opt;
    optind = 2;
    
    while ((opt = getopt(argc, argv, SHORT_OPTS)) != -1) {
        switch(opt) {
        case 'd':
            detail_level = atoi(optarg);
            break;
        case 'i':
            max_iterations = atoi(optarg);
            break;
        case 'n':
            number_of_launches = atoi(optarg);
            break;
        default:
            std::cerr << "Unknown option \"" << optopt << "\"" << std::endl;
            print_usage_str(std::cerr, argv[0]);
            return -1;
        }
    }
    
    
    
    launcher(max_iterations, number_of_launches, detail_level);
    
    
    
    return 0;
}