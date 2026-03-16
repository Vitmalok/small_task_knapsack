#include <cstdlib>
#include <ctime>
#include <iostream>

#include "solvers.hpp"
#include "knapsack.hpp"



int main() {
    std::srand(std::time(nullptr));
    
    int N, W;
    std::cin >> N >> W;
    
    knapsack::Task::Item* items = new knapsack::Task::Item[N];
    for (int i=0; i<N; ++i) {
        std::cin >> items[i].score >> items[i].weight;
    }
    
    std::cout << std::endl;
    
    knapsack::Task task(N, W, items);
    
    knapsack::Score s_best = task.impossible_score();
    knapsack::Point x_best;
    knapsack::Score s;
    
    for (int i=0; i<10; ++i) {
        /*while (solver.get_done_iterations() < 1000 && !solver.get_tolerance_reached()) {
            solver.solve(1, 1, 10);
        }*/
        Solver_local_1<knapsack::Point, knapsack::Distance, knapsack::Score> solver(task);
        solver.solve(2, 0, 100);
        
        std::cout << solver.get_done_iterations() << " iterations, result:" << std::endl;
        task.print_point_info(solver.point);
        
        s = task.score(solver.point);
        if (s_best < s) {
            s_best = s;
            x_best = solver.point;
        }
    }
    
    std::cout << std::endl << "Best point:" << std::endl;
    task.print_point_info(x_best);
    
    return 0;
}