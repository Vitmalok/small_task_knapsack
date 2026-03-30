#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

#include "randint.hpp"



namespace knapsack {

class Task {
public:
    class Point {
        friend Task;
        
        std::vector<bool> bits;
        
        Point(): bits(0) {}
    public:
        Point(const Task& task): bits(task.N) {}
    };
    
    typedef int Distance;
    typedef int Score;
    
    
    
    struct Item {
        Score score;
        int weight;
    };
    
    
    
    class Neighbourhood {
        const Task& task;
        const Point& x;
        Distance r;
        
        struct END {};
    public:
        class Iterator {
            friend Neighbourhood;
            
            const Task& task;
            const Point& x;
            Distance r;
            bool end;
            
            int k, ii;
            int* indices;
            
            Point y;
            Score s;  // Суммарный счёт, без учёта переполнения рюкзака
            int w;
            
            Iterator(const Task& _task, const Point& _x, Distance _r):
                task(_task), x(_x), r(_r), end(false),
                k(0), ii(-1), indices(nullptr),
                y(x), s(task.score(x, false)), w(task.weight(x))
            {}
            Iterator(const Task& _task, const Point& _x, Distance _r, END):
                task(_task), x(_x), r(_r), end(true),
                k(_r), ii(-1), indices(nullptr),
                y(), s(), w()
            {}
            
            void change_bit(int i) {
                if (y.bits[i]) {
                    s -= task.items[i].score;
                    w -= task.items[i].weight;
                } else {
                    s += task.items[i].score;
                    w += task.items[i].weight;
                }
                y.bits[i] = !y.bits[i];
            }
            
        public:
            ~Iterator() {
                delete[] indices;
            }
            
            std::pair<const Point&, Score> operator*() const {
                return {y, (w > task.W ? task.impossible_score() : s)};
            }
            
            bool operator!=(const Iterator& other) const {
                return !(end && other.end);
            }
            
            const Iterator& operator++() {
                ii = k-1;
                while (ii >= 0 && indices[ii] + k - ii == task.N) {
                    --ii;
                }
                if (ii >= 0) {
                    int i = indices[ii] - ii + 1;
                    for (int jj=ii; jj<k; ++jj) {
                        change_bit(indices[jj]);
                        indices[jj] = i + jj;
                        change_bit(i + jj);
                    }
                }
                
                if (ii < 0) {
                    ++k;
                    if (k > r) {
                        end = true;
                        return *this;
                    }
                    
                    for (ii=0; ii<k-1; ++ii) {
                        change_bit(indices[ii]);
                    }
                    
                    delete[] indices;
                    indices = new int[k];
                    for (ii=0; ii<k; ++ii) {
                        indices[ii] = ii;
                        change_bit(ii);
                    }
                }
                
                return *this;
            }
        };
        
        
        
        Neighbourhood(const Task& _task, const Point& _x, Distance _r):
            task(_task), x(_x), r(_r)
        {}
        
        Iterator begin() const {
            return Iterator(task, x, r);
        }
        Iterator end() const {
            return Iterator(task, x, r, END());
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
                    while (ii >= 0 && indices[ii] + k - ii == task.N) {
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
    
    
    
    class Subset {
        // friend Task;
        
        Task& task;
        int size;
        const int* indices;
        
        Score s;  // Суммарный счёт, без учёта переполнения рюкзака
        int w;
        
        struct END {};
        
        Subset(Task& _task, int _size, int* _indices, int _s, int _w):
            task(_task), size(_size), indices(_indices), s(_s), w(_w)
        {}
        
    public:
        class Iterator {
            friend Subset;
            
            const Subset& range;
            int size;
            int* indices;
            bool end;
            
            Iterator(const Subset& _range, int _i):
                range(_range), size(_range.size+1), end(false)
            {
                indices = new int[size];
                for (int i=0; i < size-1; ++i) {
                    indices[i] = _range.indices[i];
                }
                indices[size-1] = _i;
            }
            Iterator(const Subset& _range, int _i, END):
                range(_range), size(0), indices(nullptr), end(true)
            {}
        public:
            Iterator(const Iterator& other):
                range(other.range), size(other.size), end(other.end)
            {
                indices = new int[size];
                for (int i=0; i<size; ++i) {
                    indices[i] = other.indices[i];
                }
            }
            
            Iterator(Iterator&& other):
                range(other.range), size(other.size),
                indices(std::exchange(other.indices, nullptr)),
                end(other.end)
            {}
            
            ~Iterator() {
                delete[] indices;
            }
            
            const Subset operator*() const {
                Item item = range.task.items[range.task.get_sorted_indices()[indices[size-1]]];
                return Subset(range.task, size, indices,
                    range.s + item.score, range.w + item.weight
                );
            }
            
            bool operator!=(const Iterator& other) const {
                return !(end && other.end);
            }
            
            const Iterator& operator++() {
                if (size) {
                    ++indices[size-1];
                    end = indices[size-1] >= range.task.N;
                } else {
                    end = true;
                }
                return *this;
            }
        };
        
        
        
        Subset(const Subset& other):
            task(other.task), size(other.size), indices(other.indices), s(other.s), w(other.w)
        {}
        
        Subset(Task& _task):
            task(_task), size(0), indices(nullptr), s(), w()
        {}
        
        
        
        Iterator begin() const {
            if (size && indices[size-1] == task.N-1) {
                return Iterator(*this, 0, END());
            }
            return Iterator(*this, (size ? indices[size-1]+1 : 0));
        }
        Iterator end() const {
            return Iterator(*this, 0, END());
        }
        
        Point get_center() const {
            Point res(task);
            for (int i=0; i<size; ++i) {
                res.bits[task.get_sorted_indices()[indices[i]]] = true;
            }
            return res;
        }
        
        Score get_center_score() const {
            if (w > task.W) {
                return impossible_score();
            }
            return s;
        }
        
        bool is_desperate(Score best_score) const {
            if (w > task.W) {
                return true;
            }
            return false;
        }
    };
    
    
    
private:
    int N, W;
    Item* items;
    
    double* costs;
    int* sorted_indices;
    long long* pref_scores;
    long long* pref_weights;
    
    
    
public:
    static Score impossible_score() {
        return -1000000000;
    }
    
    
    
    int get_N() const {
        return N;
    }
    int get_W() const {
        return W;
    }
    const Item* get_items() const {
        return items;
    }
    
    const double* get_costs() {
        if (costs == nullptr) {
            costs = new double[N];
            for (int i=0; i<N; ++i) {
                costs[i] = (double)items[i].score / items[i].weight;
            }
        }
        return costs;
    }
    const int* get_sorted_indices() {
        if (sorted_indices == nullptr) {
            get_costs();
            sorted_indices = new int[N];
            for (int i=0; i<N; ++i) {
                sorted_indices[i] = i;
            }
            std::sort(sorted_indices, sorted_indices+N, [this](int i, int j) {return costs[i] > costs[j];});
        }
        return sorted_indices;
    }
    const long long* get_pref_scores() {
        if (pref_scores == nullptr) {
            get_sorted_indices();
            pref_scores = new long long[N+1];
            long long a = 0;
            pref_scores[0] = 0;
            for (int i=0; i<N; ++i) {
                a += items[sorted_indices[i]].score;
                pref_scores[i+1] = a;
            }
        }
        return pref_scores;
    }
    const long long* get_pref_weights() {
        if (pref_weights == nullptr) {
            get_sorted_indices();
            pref_weights = new long long[N+1];
            long long a = 0;
            pref_weights[0] = 0;
            for (int i=0; i<N; ++i) {
                a += items[sorted_indices[i]].weight;
                pref_weights[i+1] = a;
            }
        }
        return pref_weights;
    }
    
    
    
    Task():
        N(0), W(0), items(nullptr),
        costs(nullptr),
        sorted_indices(nullptr),
        pref_scores(nullptr),
        pref_weights(nullptr)
    {};
    Task(int _N, int _W, Item* _items):
        N(_N), W(_W), items(_items),
        costs(nullptr),
        sorted_indices(nullptr),
        pref_scores(nullptr),
        pref_weights(nullptr)
    {};
    
    ~Task() {
        delete[] items;
        
        delete[] costs;
        delete[] sorted_indices;
        delete[] pref_scores;
        delete[] pref_weights;
    }
    
    void load(std::istream& is) {
        delete[] items;
        
        delete[] costs;
        delete[] sorted_indices;
        delete[] pref_scores;
        delete[] pref_weights;
        
        is >> N >> W;
        
        items = new Item[N];
        for (int i=0; i<N; ++i) {
            is >> items[i].score >> items[i].weight;
        }
    }
    
    
    
    Score score(const Point& x, bool check_impossible=true) const {
        Score s = 0;
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
        if (check_impossible && w > W) {
            return impossible_score();
        }
        return s;
    }
    
    Point random_point() const {
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
    
    Subset whole_set() {
        return Subset(*this);
    }
    
    
    
    int weight(const Point& x) const {
        int w = 0;
        for (int i=0; i<N; ++i) {
            if (x.bits[i]) {
                w += items[i].weight;
            }
        }
        return w;
    }
    
    void print_point_info(std::ostream& os, const Point& x) const {
        for (int i=0; i<N; ++i) {
            os << x.bits[i];
        }
        os << std::endl;
        os << "score: " << score(x) << ", weight: " << weight(x) << std::endl;
    }
};

}