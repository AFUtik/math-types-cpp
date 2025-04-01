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

template <typename T = float, size_t N = 0, size_t M = N>
struct blerp : public blerp_data<T, N, M> {
    using blerp_data<T, N, M>::blerp_data;

    T interp(const vector<T, 2> &vec) const {
        const unsigned long long fx = vec.x / (this->p2.x - this->p1.x) / this->grid.width();  // floor x
        const unsigned long long fy = vec.y / (this->p2.y - this->p1.y) / this->grid.height(); // floor y

        return (1.0 / ((this->p2.x - this->p1.x) * (this->p2.y - this->p1.y))) * (
            this->grid.get(fx, fy)         * (this->p2.x - vec.x)     * (this->p2.y - vec.y) +
            this->grid.get(fx + 1, fy)     * (vec.x - this->p1.x) * (this->p2.y - vec.y) +
            this->grid.get(fx + 1, fy + 1) * (this->p2.x - vec.x)     * (vec.y - this->p1.y) +
            this->grid.get(fx, fy + 1)     * (vec.x - this->p1.x) * (vec.y - this->p1.y)
        );
    }
};

template <typename T = float, size_t W = 0, size_t H = W, size_t V = H>
struct tlerp : public tlerp_data<T, W, H, V> {
    using tlerp_data<T, W, H, V>::trilerp_data;

    T interp(const vector<T, 3> &vec) const {
        const unsigned long long fx = vec.x / (this->p2.x - this->p1.x) / this->grid.width();  // floor x
        const unsigned long long fy = vec.y / (this->p2.y - this->p1.y) / this->grid.height(); // floor y
        const unsigned long long fz = vec.z / (this->p2.z - this->p1.z) / this->grid.volume(); // floor z

        //const T q1 = grid.get(fx, fy, fz);
        //const T q2 = grid.get(fx + 1, fy, fz);
        //const T q3 = grid.get(fx + 1, fy + 1, fz);
        //const T q4 = grid.get(fx, fy + 1, fz);
        //const T q5 = grid.get(fx, fy, fz+1);
        //const T q6 = grid.get(fx + 1, fy, fz+1);
        //const T q7 = grid.get(fx + 1, fy + 1, fz+1);
        //const T q8 = grid.get(fx, fy + 1, fz+1);

        const T divx1 = (this->p2.x - vec.x) / (this->p2.x - this->p1.x);
        const T divx2 = (vec.x - this->p1.x) / (this->p2.x - this->p1.x);
        const T divy1 = (this->p2.y - vec.y) / (this->p2.y - this->p1.y);
        const T divy2 = (vec.y - this->p1.y) / (this->p2.y - this->p1.y);

        return (this->p2.z - vec.z) / (this->p2.z - this->p1.z) * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz) + divx2 * this->grid.get(fx + 1, fy, fz)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz) + divx2 * this->grid.get(fx, fy + 1, fz))
        ) +    (vec.z - this->p1.z) / (this->p2.z - this->p1.z) * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz + 1) + divx2 * this->grid.get(fx + 1, fy, fz + 1)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz + 1) + divx2 * this->grid.get(fx, fy + 1, fz + 1))
        );
    }
};

}

#endif