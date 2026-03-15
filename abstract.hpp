template<typename point_type, typename score_type>
class Task {
    class point_iterator;
    
    virtual score_type score(point_type x) = 0;
    
    virtual point_iterator neighbourhood(point_type x) = 0;
};

template<typename point_type, typename score_type>
class Solver {
    virtual solve(Task<point_type, score_type> task) = 0;
};