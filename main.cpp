#include "solvers.hpp"
#include "testing.hpp"

#include "knapsack.hpp"
#include "tsp.hpp"



int main() {
    //test<knapsack::Task, Solver_local<knapsack::Task>>();
    test<tsp::Task, Solver_local<tsp::Task>>(100, 1000, 0);
    
    return 0;
}