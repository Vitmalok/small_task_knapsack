#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>



template <typename Task, typename Solver>
void launch(typename Task::Distance step, int max_iterations = 100, int number_of_launches = 1, int detail_level = 2) {
    std::srand(std::time(nullptr));
    
    Task task;
    task.load(std::cin);
    
    typename Task::Score s_best = task.impossible_score();
    typename Task::Point x_best(task);
    typename Task::Score s;
    
    for (int i=0; i<number_of_launches; ++i) {
        Solver solver(task);
        
        if (detail_level >= 3) {
            solver.solve_detailed(step, max_iterations);
        } else {
            solver.solve(step, max_iterations);
        }
        
        if (detail_level >= 2) {
            std::cout << solver.get_done_iterations() << " iterations, result:" << std::endl;
            task.print_point_info(solver.point);
            std::cout << std::endl;
        } else if (detail_level >= 1) {
            std::cout << solver.get_done_iterations() << " iterations" << std::endl;
        }
        
        s = task.score(solver.point);
        if (s_best < s) {
            s_best = s;
            x_best = solver.point;
        }
    }
    
    if (detail_level >= 1) {
        std::cout << std::endl;
    }
    std::cout << "Best point:" << std::endl;
    task.print_point_info(x_best);
}