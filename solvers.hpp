#pragma once



template <typename Task>
class Solver_local {
    Task& task;
    
    int done_iterations;
    bool tolerance_reached;
    
public:
    int get_done_iterations() {
        return done_iterations;
    }
    int get_tolerance_reached() {
        return tolerance_reached;
    }
    
    typename Task::Point point;
    
    Solver_local(Task& _task):
        task(_task),
        done_iterations(0),
        tolerance_reached(false),
        point(_task.random_point())
    {}
    
    typename Task::Point solve(typename Task::Distance step, typename Task::Score tolerance, int max_iterations) {
        typename Task::Score s = task.score(point);
        typename Task::Score s_prev = task.impossible_score();
        
        int it = 0;
        while (it != max_iterations && s - s_prev > tolerance) {
            s_prev = s;
            for (typename Task::Point point_next : task.neighbourhood(point, step)) {
                typename Task::Score s_next = task.score(point_next);
                if (s < s_next) {
                    point = point_next;
                    s = s_next;
                }
            }
            ++it;
        }
        
        done_iterations += it;
        if (s - s_prev <= tolerance) {
            tolerance_reached = true;
        }
        
        return point;
    }
};