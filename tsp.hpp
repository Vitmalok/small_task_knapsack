#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include "randint.hpp"



namespace tsp {

class Task {
public:
    struct Vertex {
        double x;
        double y;
    };
    
    class Point {
        friend Task;
        
        std::vector<int> indices;
        
        Point(): indices(0) {}
    public:
        Point(const Task& task): indices(task.N) {
            for (int i=0; i<task.N; ++i) {
                indices[i] = i;
            }
        }
    };
    
    typedef int Distance;
    typedef double Score;
    
    
    
    class Neighbourhood {
        const Task& task;
        const Point& x;
        Distance r;
    public:
        struct END {};
        
        class Iterator {
            friend Neighbourhood;
            
            const Task& task;
            const Point& x;
            Distance r;
            bool end;
            
            int i1, i2;
            
            Point y;
            
            Iterator(const Task& _task, const Point& _x, Distance _r, END):
                task(_task), x(_x), r(_r), end(true),
                i1(_task.N-1), i2(task.N),
                y()
            {}
        public:
            Iterator(const Task& _task, const Point& _x, Distance _r):
                task(_task), x(_x), r(_r), end(false),
                i1(1), i2(2),
                y(x)
            {}
            
            const Point& operator*() {
                return y;
            }
            
            bool operator!=(const Iterator& other) {
                return !(end && other.end);
            }
            
            const Iterator& operator++() {
                if (r == 2) {  // 2-opt
                    int ia, ib, ic, id;
                    
                    do {
                        ++i2;
                        if (i2 >= (i1 == 1 ? task.N-1 : task.N)) {
                            ++i1;
                            if (i1 >= task.N-1) {
                                end = true;
                                return *this;
                            }
                            i2 = i1+1;
                        }
                        
                        ia = x.indices[i1-1];
                        ib = x.indices[i1];
                        ic = x.indices[i2];
                        id = x.indices[(i2+1 == task.N ? 0 : i2+1)];
                    } while (task.edge_length(ia, ib) + task.edge_length(ic, id) - task.edge_length(ia, ic) - task.edge_length(ib, id) < 0);
                    
                    y = x;
                    int i=i1, j=i2;
                    while (i < j) {
                        std::swap(y.indices[i], y.indices[j]);
                        ++i; --j;
                    }
                    
                    return *this;
                } else if (r == 3) {  // 3-opt
                    //...
                }
                
                end = true;
                return *this;
            }
        };
        
        
        
        Neighbourhood(const Task& _task, const Point& _x, Distance _r):
            task(_task), x(_x), r(_r)
        {}
        
        Iterator begin() {
            return Iterator(task, x, r);
        }
        
        Iterator end() {
            return Iterator(task, x, r, END());
        }
        
        // Предыдущая реализация
        std::vector<Point> to_vector(const Point& x, Distance r) {
            std::vector<Point> res;
            res.push_back(x);
            
            if (r == 2) {  // 2-opt
                for (int i1=1; i1 < task.N-1; ++i1) {
                    for (int i2=i1+1; i2 < (i1 == 1 ? task.N-1 : task.N); ++i2) {
                        int ia = x.indices[i1-1];
                        int ib = x.indices[i1];
                        int ic = x.indices[i2];
                        int id = x.indices[(i2+1 == task.N ? 0 : i2+1)];
                        
                        if (task.edge_length(ia, ib) + task.edge_length(ic, id) - task.edge_length(ia, ic) - task.edge_length(ib, id) < 0){
                            continue;
                        }
                        
                        Point y = x;
                        
                        int i=i1, j=i2;
                        while (i < j) {
                            std::swap(y.indices[i], y.indices[j]);
                            ++i; --j;
                        }
                        
                        res.push_back(y);
                    }
                }
            } else if (r == 3) {  // 3-opt
                //...
            }
            
            return res;
        }
    };
    
    
    
private:
    int N;
    Vertex* vertices;

public:
    Task():
        N(0), vertices(nullptr)
    {};
    Task(int _N, Vertex* _vertices):
        N(_N), vertices(_vertices)
    {};
    
    ~Task() {
        delete[] vertices;
    }
    
    void load(std::istream& is) {
        delete[] vertices;
        
        is >> N;
        
        vertices = new Vertex[N];
        for (int i=0; i<N; ++i) {
            is >> vertices[i].x >> vertices[i].y;
        }
    }
    
    
    
    Score edge_length(int i1, int i2) const {
        Vertex& v1 = vertices[i1];
        Vertex& v2 = vertices[i2];
        Score dx = v1.x - v2.x;
        Score dy = v1.y - v2.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    Score path_length(const Point& x) const {
        Score s = edge_length(x.indices[0], x.indices[N-1]);
        for (int i=1; i<N; ++i) {
            s += edge_length(x.indices[i], x.indices[i-1]);
        }
        return s;
    }
    
    
    
    Score impossible_score() const {
        return -INFINITY;
    }
    
    Score score(const Point& x) const {
        return -path_length(x);
    }
    
    Point random_point() const {
        Point x(*this);
        
        for (int i=1; i<N-1; ++i) {
            std::swap(x.indices[i], x.indices[randint(i, N)]);
        }
        if (x.indices[1] > x.indices[N-1]) {
            std::swap(x.indices[1], x.indices[N-1]);
        }
        
        return x;
    }
    
    
    
    void print_point_info(const Point& x) const {
        Point y = x;
        if (y.indices[1] > y.indices[N-1]) {
            int i=1, j=N-1;
            while (i < j) {
                std::swap(y.indices[i], y.indices[j]);
                ++i; --j;
            }
        }
        
        for (int i=0; i<N-1; ++i) {
            std::cout << y.indices[i] << " ";
        }
        std::cout << y.indices[N-1] << std::endl;
        
        std::cout << "length: " << path_length(y) << std::endl;
    }
};

}