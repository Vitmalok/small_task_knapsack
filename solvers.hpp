#pragma once



template <typename Task>
class Solver_local {
    Task& task;
    
    int done_iterations;
    
public:
    int get_done_iterations() const {
        return done_iterations;
    }
    
    typename Task::Point point;
    
    Solver_local(Task& _task):
        task(_task),
        done_iterations(0),
        point(_task.random_point())
    {}
    
    typename Task::Point solve(typename Task::Distance step, int max_iterations) {
        typename Task::Score s = task.score(point);
        typename Task::Score s_prev = task.impossible_score();
        
        int it = 0;
        while (it != max_iterations && (s > s_prev || s == task.impossible_score())) {
            s_prev = s;
            //std::cout << std::endl;
            //task.print_point_info(point);
            for (std::pair<const typename Task::Point&, typename Task::Score> pair : typename Task::Neighbourhood(task, point, step)) {
                //task.print_point_info(pair.first);
                if (s < pair.second || s == pair.second && s == task.impossible_score()) {
                    point = pair.first;
                    s = pair.second;
                }
            }
            ++it;
        }
        
        done_iterations += it;
        
        return point;
    }
    
    typename Task::Point solve_detailed(typename Task::Distance step, int max_iterations) {
        while (done_iterations < max_iterations) {
            task.print_point_info(point);
            solve(step, 1);
        }
        
        return point;
    }
};



/*template <typename Task>
class Solver_bnb {
    Task& task;
    
    int done_iterations;
    
public:
    int get_done_iterations() const {
        return done_iterations;
    }
    
    typename Task::Point point;
    
    Solver_local(Task& _task):
        task(_task),
        done_iterations(0),
        tolerance_reached(false),
        point(_task.random_point())
    {}
    
    typename Task::Point solve(typename Task::Distance step, int max_iterations) {
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
    
    typename Task::Point solve_detailed(typename Task::Distance step, typename Task::Score tolerance, int max_iterations) {
        while (done_iterations < max_iterations && !tolerance_reached) {
            task.print_point_info(point);
            solve(step, tolerance, 1);
        }
        
        return point;
    }
};*/