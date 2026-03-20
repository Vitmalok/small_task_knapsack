#include "knapsack.hpp"
#include "solvers.hpp"
#include "testing.hpp"



int main() {
    test<knapsack::Task, Solver_local<knapsack::Task>>();
    
    return 0;
}