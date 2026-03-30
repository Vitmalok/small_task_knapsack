#pragma once

#include <deque>

#include "indent_ostream.hpp"



template <typename Task>
class Solver_local {
    Task& task;
    typename Task::Point point;
    int done_iterations;
    bool is_solved;
    
public:
    const typename Task::Point& get_point() const {
        return point;
    }
    int get_done_iterations() const {
        return done_iterations;
    }
    bool get_is_solved() const {
        return is_solved;
    }
    
    void set_point(const typename Task::Point& _point) {
        point = _point;
    }
    
    
    
    Solver_local(Task& _task, typename Task::Score best_score):
        task(_task),
        point(_task.random_point()),
        done_iterations(0),
        is_solved(false)
    {}
    
    typename Task::Point solve(typename Task::Distance step, int max_iterations) {
        typename Task::Score s = task.score(point);
        typename Task::Score s_prev = task.impossible_score();
        
        int iterations = 0;
        while ((max_iterations == -1 || iterations < max_iterations) && (s > s_prev || s == task.impossible_score())) {
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
            ++iterations;
        }
        
        done_iterations += iterations;
        is_solved = s <= s_prev && s != task.impossible_score();
        
        return point;
    }
    
    typename Task::Point solve_detailed(std::ostream& os, typename Task::Distance step, int max_iterations) {
        while (done_iterations < max_iterations && !is_solved) {
            task.print_point_info(os, point);
            solve(step, 1);
        }
        
        return point;
    }
};



template <typename Task>
class Solver_bnb {
    Task& task;
    
    int done_iterations;
    bool is_solved;
    
    std::deque<typename Task::Subset> subset_stack;
    std::deque<typename Task::Subset::Iterator> it_stack;
    
    typename Task::Point point;
    typename Task::Score best_score;
    
public:
    const typename Task::Point& get_point() const {
        return point;
    }
    int get_done_iterations() const {
        return done_iterations;
    }
    bool get_is_solved() const {
        return is_solved;
    }
    
    
    
    Solver_bnb(Task& _task, typename Task::Score _best_score):
        task(_task),
        done_iterations(0),
        is_solved(false),
        subset_stack({task.whole_set()}),
        it_stack({subset_stack.back().begin()}),
        point(subset_stack.back().get_center()),
        best_score(_best_score)
    {
        //subset_stack.push_back(task.whole_set());
        //it_stack.push_back(subset_stack.back().begin());
        //point = subset_stack.back().get_center();
    }
    
    typename Task::Point solve(typename Task::Distance step, int max_iterations) {
        int iterations = 0;
        while (!it_stack.empty() && (max_iterations == -1 || iterations < max_iterations)) {
            typename Task::Subset::Iterator end = subset_stack.back().end();
            typename Task::Subset::Iterator& it = it_stack.back();
            
            while (it != end && (*it).is_desperate(best_score)) {
                ++it;
            }
            
            if (it != end) {
                subset_stack.push_back(*it);
                it_stack.emplace_back(std::move(subset_stack.back().begin()));
            } else {
                typename Task::Score s = subset_stack.back().get_center_score();
                
                if (best_score < s) {
                    best_score = s;
                    point = subset_stack.back().get_center();
                }
                
                subset_stack.pop_back();
                it_stack.pop_back();
                
                if (!it_stack.empty()) {
                    ++it_stack.back();
                }
            }
            
            ++iterations;
        }
        
        done_iterations += iterations;
        is_solved = it_stack.empty();
        
        return point;
    }
    
    typename Task::Point solve_detailed(std::ostream& os, typename Task::Distance step, int max_iterations) {
        while (done_iterations < max_iterations && !is_solved) {
            os << "bnb subset_stack centers:" << std::endl;
            for (const typename Task::Subset& subset : subset_stack) {
                task.print_point_info(os, subset.get_center());
            }
            os << std::endl;
            
            solve(step, 1);
        }
        
        return point;
    }
};