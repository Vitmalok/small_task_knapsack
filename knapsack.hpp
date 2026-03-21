#pragma once

#include <iostream>
#include <vector>

#include "randint.hpp"



namespace knapsack {

class Task {
public:
    struct Item {
        int score;
        int weight;
    };
    
    class Point {
        std::vector<bool> bits;
    public:
        Point(Task& task): bits(task.N) {}
        friend Task;
    };
    
    typedef int Distance;
    typedef int Score;
    
private:
    int N, W;
    Item* items;

public:
    Task():
        N(0), W(0), items(nullptr)
    {};
    Task(int _N, int _W, Item* _items):
        N(_N), W(_W), items(_items)
    {};
    
    ~Task() {
        delete[] items;
    }
    
    void load(std::istream& is) {
        delete[] items;
        
        is >> N >> W;
        
        items = new Item[N];
        for (int i=0; i<N; ++i) {
            is >> items[i].score >> items[i].weight;
        }
    }
    
    
    
    Score impossible_score() {
        return -1000000000;
    }
    
    Score score(const Point& x) {
        int s = 0;
        int w = 0;
        for (int i=0; i<N; ++i) {
            if (x.bits[i]) {
                s += items[i].score;
                w += items[i].weight;
                if (w > W) {
                    i = N;
                }
            }
        }
        if (w > W) {
            return impossible_score();
        }
        return s;
    }
    
    std::vector<Point> neighbourhood(const Point& x, Distance r) {
        std::vector<Point> res;
        res.push_back(x);
        
        for (int k=1; k<=r; ++k) {
            int indices[k];
            int ii;
            for (ii=0; ii<k; ++ii) {
                indices[ii] = ii;
            }
            
            do {
                Point y = x;
                for (ii=0; ii<k; ++ii) {
                    y.bits[indices[ii]] = !y.bits[indices[ii]];
                }
                res.push_back(y);
                
                ii = k-1;
                while (ii >= 0 && indices[ii] + k - ii == N) {
                    --ii;
                }
                if (ii >= 0) {
                    int i = indices[ii] - ii + 1;
                    for (int jj=ii; jj<k; ++jj) {
                        indices[jj] = i + jj;
                    }
                }
            } while (ii >= 0);
        }
        
        return res;
    }
    
    Point random_point() {
        Point x(*this);
        int w;
        int w_max = W/2;
        
        int it = 0;
        while (it < N && (w = weight(x)) < w_max) {
            int i = randint(0, N);
            
            if (!x.bits[i] && w + items[i].weight <= w_max) {
                x.bits[i] = true;
            }
            ++it;
        }
        
        return x;
    }
    
    
    
    int weight(const Point& x) {
        int w = 0;
        for (int i=0; i<N; ++i) {
            if (x.bits[i]) {
                w += items[i].weight;
            }
        }
        return w;
    }
    
    void print_point_info(const Point& x) {
        for (int i=0; i<N; ++i) {
            std::cout << x.bits[i];
        }
        std::cout << std::endl;
        std::cout << "score: " << score(x) << ", weight: " << weight(x) << std::endl;
    }
};

}