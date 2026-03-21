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
        std::vector<int> indices;
    public:
        Point(Task& task): indices(task.N) {
            for (int i=0; i<task.N; ++i) {
                indices[i] = i;
            }
        }
        friend Task;
    };
    
    typedef int Distance;
    typedef double Score;
    
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
    
    
    
    Score edge_length(int i1, int i2) {
        Vertex& v1 = vertices[i1];
        Vertex& v2 = vertices[i2];
        Score dx = v1.x - v2.x;
        Score dy = v1.y - v2.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    Score path_length(const Point& x) {
        Score s = edge_length(x.indices[0], x.indices[N-1]);
        for (int i=1; i<N; ++i) {
            s += edge_length(x.indices[i], x.indices[i-1]);
        }
        return s;
    }
    
    
    
    Score impossible_score() {
        return -INFINITY;
    }
    
    Score score(const Point& x) {
        return -path_length(x);
    }
    
    std::vector<Point> neighbourhood(const Point& x, Distance r) {
        std::vector<Point> res;
        res.push_back(x);
        
        if (r == 2) {  // 2-opt
            for (int i1=1; i1<N-1; ++i1) {
                for (int i2=i1+1; i2<(i1 == 1 ? N-1 : N); ++i2) {
                    int ia = x.indices[i1-1];
                    int ib = x.indices[i1];
                    int ic = x.indices[i2];
                    int id = x.indices[(i2+1 == N ? 0 : i2+1)];
                    
                    if (edge_length(ia, ib) + edge_length(ic, id) - edge_length(ia, ic) - edge_length(ib, id) < 0){
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
    
    Point random_point() {
        Point x(*this);
        
        for (int i=1; i<N-1; ++i) {
            std::swap(x.indices[i], x.indices[randint(i, N)]);
        }
        if (x.indices[1] > x.indices[N-1]) {
            std::swap(x.indices[1], x.indices[N-1]);
        }
        
        return x;
    }
    
    
    
    void print_point_info(const Point& x) {
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