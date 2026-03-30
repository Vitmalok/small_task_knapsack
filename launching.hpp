#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>



template <typename Task, typename Solver>
void launch(
    std::istream& is, std::ostream& os,
    typename Task::Distance step, typename Task::Score best_score = Task::impossible_score(),
    int max_iterations = 100, int number_of_launches = 1, int detail_level = 2
) {
    std::srand(std::time(nullptr));
    
    Task task;
    task.load(is);
    
    typename Task::Score s_best = task.impossible_score();
    typename Task::Point x_best(task);
    typename Task::Score s;
    
    for (int i=0; i<number_of_launches; ++i) {
        Solver solver(task, best_score);
        
        if (detail_level >= 3) {
            solver.solve_detailed(os, step, max_iterations);
        } else {
            solver.solve(step, max_iterations);
        }
        
        if (detail_level >= 2) {
            os << solver.get_done_iterations() << " iterations, result:" << std::endl;
            task.print_point_info(os, solver.get_point());
            os << std::endl;
        } else if (detail_level >= 1) {
            os << solver.get_done_iterations() << " iterations" << std::endl;
        }
        
        s = task.score(solver.get_point());
        if (s_best < s) {
            s_best = s;
            x_best = solver.get_point();
        }
    }
    
    if (detail_level >= 1) {
        os << std::endl;
    }
    os << "Best point:" << std::endl;
    task.print_point_info(os, x_best);
}