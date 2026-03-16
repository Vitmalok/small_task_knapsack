#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include "abstract.hpp"



int randint(int a, int b) {
    // Генерирует случайное целое число в полуинтервале [a, b)
    return a + (int)((double)rand() / ((double)RAND_MAX + 1) * (double)(b-a));
}



namespace knapsack {

typedef std::vector<bool> Point;
typedef int Distance;
typedef int Score;

class Task: public abstract::Task<Point, Distance, Score> {
public:
    struct Item {
        int score;
        int weight;
    };
    
private:
    int N, W;
    Item* items;

public:
    Task(int _N, int _W, Item* _items):
        N(_N), W(_W), items(_items)
    {};
    
    ~Task() {
        delete[] items;
    }
    
    virtual Score impossible_score() override {
        return -1000000000;
    }
    
    virtual Score score(const Point& x) override {
        int s = 0;
        int w = 0;
        for (int i=0; i<N; ++i) {
            if (x[i]) {
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
    
    virtual std::vector<Point> neighbourhood(const Point& x, Distance r) override {
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
                    y[indices[ii]] = !y[indices[ii]];
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
    
    virtual Point random_point() override {
        Point x(N);
        int w;
        int w_max = W/2;
        
        int it = 0;
        while (it < N && (w = weight(x)) < w_max) {
            int i = randint(0, N);
            if (!x[i] && w + items[i].weight <= w_max) {
                x[i] = true;
            }
            ++it;
        }
        
        return x;
    }
    
    
    
    int weight(const Point& x) {
        int w = 0;
        for (int i=0; i<N; ++i) {
            if (x[i]) {
                w += items[i].weight;
            }
        }
        return w;
    }
    
    void print_point_info(const Point& x) {
        for (int i=0; i<N; ++i) {
            std::cout << x[i];
        }
        std::cout << std::endl;
        std::cout << "score: " << score(x) << ", weight: " << weight(x) << std::endl;
    }
};

}