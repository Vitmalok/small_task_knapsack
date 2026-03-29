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
        friend Task;
        
        std::vector<bool> bits;
        
        Point(): bits(0) {}
    public:
        Point(const Task& task): bits(task.N) {}
    };
    
    typedef int Distance;
    typedef int Score;
    
    
    
    class Neighbourhood {
        int N;
        const Point& x;
        Distance r;
    public:
        struct END {};
        
        class Iterator {
            friend Neighbourhood;
            
            int N;
            const Point& x;
            Distance r;
            bool end;
            
            int k, ii;
            int* indices;
            
            Point y;
            
            Iterator(int _N, const Point& _x, Distance _r, END):
                N(_N), x(_x), r(_r), end(true),
                k(_r), ii(-1), indices(nullptr),
                y()
            {}
        public:
            Iterator(int _N, const Point& _x, Distance _r):
                N(_N), x(_x), r(_r), end(false),
                k(0), ii(-1), indices(nullptr),
                y(x)
            {}
            ~Iterator() {
                delete[] indices;
            }
            
            const Point& operator*() {
                return y;
            }
            
            bool operator!=(const Iterator& other) {
                return !(end && other.end);
            }
            
            const Iterator& operator++() {
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
                
                if (ii < 0) {
                    ++k;
                    if (k > r) {
                        end = true;
                        return *this;
                    }
                    
                    delete[] indices;
                    indices = new int[k];
                    for (ii=0; ii<k; ++ii) {
                        indices[ii] = ii;
                    }
                }
                
                y = x;
                for (ii=0; ii<k; ++ii) {
                    y.bits[indices[ii]] = !y.bits[indices[ii]];
                }
                
                return *this;
            }
        };
        
        
        
        Neighbourhood(const Task& task, const Point& _x, Distance _r):
            N(task.N), x(_x), r(_r)
        {}
        
        Iterator begin() {
            return Iterator(N, x, r);
        }
        
        Iterator end() {
            return Iterator(N, x, r, END());
        }
        
        // Предыдущая реализация
        std::vector<Point> to_vector() {
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
    };
    
    
    
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
    
    Point random_point() {
        Point x(*this);
        int w;
        int w_max = randint(W/2, W+1);
        
        int it = 0;
        while (it < N && (w = weight(x)) < w_max) {
            int i = randint(0, N);
            
            //if (!x.bits[i] && w + items[i].weight <= w_max) {
            if (!x.bits[i]) {
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