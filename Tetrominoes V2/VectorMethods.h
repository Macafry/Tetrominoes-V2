#pragma once
//NumericType
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include <random>
#include <chrono>
#include <limits>
#include <type_traits>

using std::string;
using std::vector;

template <typename T>
string vectorToString(vector<T> vec) {
    std::ostringstream oss;

    if (!vec.empty())
    {
        
        std::copy(vec.begin(), vec.end() - 1,
            std::ostream_iterator<T>(oss, ", "));

       
        oss << vec.back();
    }

    return "[" + oss.str() + "]\n";
}

template <typename T>
string vectorToStringNNL(vector<T> vec) {
    std::ostringstream oss;

    if (!vec.empty())
    {

        std::copy(vec.begin(), vec.end() - 1,
            std::ostream_iterator<T>(oss, ", "));


        oss << vec.back();
    }

    return "[" + oss.str() + "]";
}

template <typename T>
int getPos(vector<T> vec, T item) {
    auto it = std::find(vec.begin(), vec.end(), item);
    return it == vec.end() ? -1 : it-vec.begin();
}

template <typename T>
void deleteItem(vector<T>* vec, T item) {
    auto pos = getPos(*vec, item);
    if (pos != -1) {
        vec->erase(vec->begin() + pos);
    }
}

template <typename T>
void shuffle(vector<T>* vec) {
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(vec->begin(), vec->end(), rng);
}

template <typename T>
vector<T> shuffle(vector<T> vec) {
    shuffle(&vec);
    return vec;
}

template <typename T>
void sort(vector<T>* vec) {
    std::sort(vec->begin(), vec->end());
}

template <typename T>
vector<T> sort(vector<T> vec) {
    std::sort(std::begin(vec), std::end(vec));
    return vec;
}

template <typename T>
void swap(vector<T>* vec, int i, int j) {
    T temp = vec->at(i);
    vec->at(i) = vec->at(j);
    vec->at(j) = temp;
}

template <typename T>
void reverse(vector<T>* vec) {
    std::reverse(vec->begin(), vec->end());
}

template <typename T>
vector<T> reverse(vector<T> vec) {
    std::reverse(vec.begin(), vec.end());
    return vec;
}

template <typename NumericType>
NumericType minimum(vector<NumericType> vec) {
    auto min = std::numeric_limits<NumericType>::max();
    for (auto& i : vec)
    {
        if (i < min) {
            min = i;
        }
    }
    return min;
}

template <typename NumericType>
NumericType maximum(vector<NumericType> vec) {
    auto max = std::numeric_limits<NumericType>::min();
    for (auto& i : vec)
    {
        if (i < max) {
            max = i;
        }
    }
    return max;
}

template <typename NumericType>
int xAreSmaller(vector<NumericType> vec, NumericType item) {
    int c = 0;
    for  (auto& i : vec )
    {
        if (i < item) {
            c++;
        }
    }
    return c;
}

template <typename NumericType>
int xAreLarger(vector<NumericType> vec, NumericType item) {
    int c = 0;
    for (auto& i : vec)
    {
        if (i > item) {
            c++;
        }
    }
    return c;
}

template <typename NumericType>
NumericType kRank(vector<NumericType> vec, int rank) {
    auto sVec = sort(vec);
    return sVec[rank];
}

template <typename NumericType>
NumericType median(vector<NumericType> vec) {
    return kRank(vec, (vec.size() - 1) / 2);
}

template <typename NumericType>
double trueMedian(vector<NumericType> vec) {
    int s = vec.size();
    if (s % 2) {
        return kRank(vec, s / 2);
    }

    return 0.5 * (kRank(vec, s / 2) + kRank(vec, s / 2 - 1));
    
}

template <typename NumericType>
int isRankK(vector<NumericType> vec, NumericType item, int k) {
    int Xs = xAreSmaller(vec, item);
    int Xl = xAreLarger(vec, item);

    if (Xs <= k and k < (vec.size() - Xl)) {
        return 0;
    }

    if (k < Xs) {
        return Xs - k;
    }

    return k - Xl + 1;

}
