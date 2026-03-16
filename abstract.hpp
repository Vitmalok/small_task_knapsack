#pragma once

#include <vector>

namespace abstract {

template<typename Point, typename Distance, typename Score>
class Task {
public:
    virtual Score impossible_score() = 0;
    
    virtual Score score(const Point& x) = 0;
    
    virtual std::vector<Point> neighbourhood(const Point& x, Distance r) = 0;
    
    virtual Point random_point() = 0;
};

template<typename Point, typename Distance, typename Score>
class Solver {
public:
    virtual Point solve(Distance step, Score tolerance, int max_iterations = -1) = 0;
};

}