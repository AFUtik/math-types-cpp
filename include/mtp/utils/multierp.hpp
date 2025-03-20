#ifndef MULTIERP_HPP
#define MULTIERP_HPP

#include "lerp.hpp"
#include "../matrix.hpp"

namespace mtpu {

template <typename T = float, size_t N = 0, size_t M = N>
struct blerp_data { /* Bilinear Interpolation Data */
    vector<T, 2> p1, p2;
    matrix<T, N, M> grid; // Can be static or dynamic //
    
    /*                     GRID SCHEME
    *   (X: 0.0, Y: 1.0) - Q3=====Q4 - p2(X: 1.0, Y: 1.0)
    *                      ||     ||
    *                      ||     ||
    * p1(X: 0.0, Y: 0.0) - Q1=====Q2 - (X: 1.0, Y: 0.0)
    */

    constexpr blerp_data(const vector2<T> &start, const vector2<T> &end) : p1(start), p2(end) {}

    constexpr blerp_data(const T& x, const T& y) : p1(0), p2(x, y) {}
    
    constexpr blerp_data() : p1(0), p2(1) {}
};

template <typename T = float, size_t W = 0, size_t H = W, size_t V = H>
struct tlerp_data {  /* Trilinear Interpolation Data */
    vector<T, 3> p1, p2;
    matrix3d<T, W, H, V> grid;

    /*                   GRID SCHEME
    *                    Q7------Q8 - p2(X: 1.0, Y: 1.0, Z: 1.0)
    *                     |\      | \
    *  (X: 0.0, Y: 1.0) - | Q3----|--\Q4 - (X: 1.0, Y: 1.0)
    *                     | |     |  |
    *                    Q5------Q6  |
    *                      \|       \|
    *                       \--------\
    *  p1(X: 0.0, Y: 0.0) - Q1        Q2 - (X: 1.0, Y: 0.0)
    */

    constexpr tlerp_data(const vector3<T> &start, const vector3<T> &end) : p1(start), p2(end) {}

    constexpr tlerp_data(const T& x, const T& y, const T& z) : p1(0), p2(x, y, z) {}
    
    constexpr tlerp_data() : p1(0), p2(1) {}
};

template <typename T = float>
struct blerp : public blerp_data<T> {
    using blerp_data<T>::blerp_data;

    T interp(const vector<T, 2> &vec) {

    }
};

template <typename T = float>
struct tlerp : public blerp_data<T> {
    using tlerp_data<T>::trilerp_data;

    T interp(const vector<T, 3> &vec) {
        
    }
};

}

#endif