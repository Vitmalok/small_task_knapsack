#include <cstdlib>
#include <ctime>
#include <iostream>



template <typename Task, typename Solver>
void test() {
    std::srand(std::time(nullptr));
    
    Task task;
    task.load(std::cin);
    
    std::cout << std::endl;
    
    typename Task::Score s_best = task.impossible_score();
    typename Task::Point x_best(task);
    typename Task::Score s;
    
    for (int i=0; i<10; ++i) {
        Solver solver(task);
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
}