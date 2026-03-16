#pragma once

#include "abstract.hpp"



template<typename Point, typename Distance, typename Score>
class Solver_local_1: public abstract::Solver<Point, Distance, Score> {
    abstract::Task<Point, Distance, Score>& task;
    
    int done_iterations;
    bool tolerance_reached;
    
public:
    int get_done_iterations() {
        return done_iterations;
    }
    int get_tolerance_reached() {
        return tolerance_reached;
    }
    
    Point point;
    
    Solver_local_1(abstract::Task<Point, Distance, Score>& _task):
        task(_task),
        done_iterations(0),
        tolerance_reached(false),
        point(_task.random_point())
    {};
    
    virtual Point solve(Distance step, Score tolerance, int max_iterations) override {
        Score s = task.score(point);
        Score s_prev = task.impossible_score();
        
        int it = 0;
        while (it != max_iterations && s - s_prev > tolerance) {
            s_prev = s;
            for (Point point_next : task.neighbourhood(point, step)) {
                Score s_next = task.score(point_next);
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