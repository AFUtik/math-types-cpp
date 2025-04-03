#ifndef MULTIERP_HPP
#define MULTIERP_HPP

#include "lerp.hpp"
#include "../matrix.hpp"

#include <iostream>

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
        const T subx = this->p2.x - this->p1.x;
        const T suby = this->p2.y - this->p1.y;

        const unsigned long long fx = vec.x / subx / this->grid.width();  // floor x
        const unsigned long long fy = vec.y / suby / this->grid.height(); // floor y

        return (1.0 / (subx * suby)) * (
            this->grid.get(fx, fy)         * (this->p2.x - vec.x) * (this->p2.y - vec.y) +
            this->grid.get(fx + 1, fy)     * (vec.x - this->p1.x) * (this->p2.y - vec.y) +
            this->grid.get(fx + 1, fy + 1) * (this->p2.x - vec.x) * (vec.y - this->p1.y) +
            this->grid.get(fx, fy + 1)     * (vec.x - this->p1.x) * (vec.y - this->p1.y)
        );
    }

    T interp(T x, T y) const {
        const T subx = this->p2.x - this->p1.x;
        const T suby = this->p2.y - this->p1.y;

        const unsigned long long fx = x / subx / this->grid.width();  // floor x
        const unsigned long long fy = y / suby / this->grid.height(); // floor y

        return (1.0 / (subx * suby)) * (
            this->grid.get(fx, fy)         * (this->p2.x - x) * (this->p2.y - y) +
            this->grid.get(fx + 1, fy)     * (x - this->p1.x) * (this->p2.y - y) +
            this->grid.get(fx + 1, fy + 1) * (this->p2.x - x) * (y - this->p1.y) +
            this->grid.get(fx, fy + 1)     * (x - this->p1.x) * (y - this->p1.y)
            );
    }

    template <size_t N, size_t M>
    void discrete(matrix<T, N, M> &mat) {
        if (mat.data == nullptr) return;

        const size_t szx = mat.width();
        const size_t szy = mat.height();

        const T dx = this->p2.x / (szx-1);
        const T dy = this->p2.y / (szy-1);
        for (size_t cx = 0; cx < szx; cx++) {
            for (size_t cy = 0; cy < szy; cy++) {
                mat.get(cx, cy) = interp(cx * dx, cy * dy);
            }
        }
    }

    matrix<T, 0> discrete(size_t szx, size_t szy) {
        matrix<T, 0> mat(szx, szy);
        const T dx = this->p2.x / (szx - 1);
        const T dy = this->p2.y / (szy - 1);
        for (size_t cx = 0; cx < szx; cx++) {
            for (size_t cy = 0; cy < szy; cy++) {
                mat.get(cx, cy) = interp(cx * dx, cy * dy);
            }
        }
        return mat;
    }

    template <size_t N, size_t M>
    matrix<T, N, M> discrete() {
        matrix<T, N, M> mat;
        const T dx = this->p2.x / (N - 1);
        const T dy = this->p2.y / (M - 1);
        for (size_t cx = 0; cx < N; cx++) {
            for (size_t cy = 0; cy < M; cy++) {
                mat.get(cx, cy) = interp(cx * dx, cy * dy);
            }
        }
        return mat;
    }
};

template <typename T = float, size_t W = 0, size_t H = W, size_t V = H>
struct tlerp : public tlerp_data<T, W, H, V> {
    using tlerp_data<T, W, H, V>::tlerp_data;

    T interp(const vector<T, 3> &vec) const {
        const T subx = this->p2.x - this->p1.x;
        const T suby = this->p2.y - this->p1.y;
        const T subz = this->p2.z - this->p1.z;

        const unsigned long long fx = vec.x / subx / this->grid.width();  // floor x
        const unsigned long long fy = vec.y / suby / this->grid.height(); // floor y
        const unsigned long long fz = vec.z / subz / this->grid.volume(); // floor z

        const T divx1 = (this->p2.x - vec.x) / subx;
        const T divx2 = (vec.x - this->p1.x) / subx;
        const T divy1 = (this->p2.y - vec.y) / suby;
        const T divy2 = (vec.y - this->p1.y) / suby;

        return (this->p2.z - vec.z) / subz * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz) + divx2 * this->grid.get(fx + 1, fy, fz)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz) + divx2 * this->grid.get(fx, fy + 1, fz))
        ) +    (vec.z - this->p1.z) / subz * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz + 1) + divx2 * this->grid.get(fx + 1, fy, fz + 1)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz + 1) + divx2 * this->grid.get(fx, fy + 1, fz + 1))
        );
    }

    T interp(T x, T y, T z) const {
        const T subx = this->p2.x - this->p1.x;
        const T suby = this->p2.y - this->p1.y;
        const T subz = this->p2.z - this->p1.z;

        const unsigned long long fx = x / subx / this->grid.width();  // floor x
        const unsigned long long fy = y / suby / this->grid.height(); // floor y
        const unsigned long long fz = z / subz / this->grid.volume(); // floor z

        const T divx1 = (this->p2.x - x) / subx;
        const T divx2 = (x - this->p1.x) / subx;
        const T divy1 = (this->p2.y - y) / suby;
        const T divy2 = (y - this->p1.y) / suby;

        return (this->p2.z - z) / subz * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz) + divx2 * this->grid.get(fx + 1, fy, fz)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz) + divx2 * this->grid.get(fx, fy + 1, fz))
        ) + (z - this->p1.z) / subz * (
            divy1 * (divx1 * this->grid.get(fx, fy, fz + 1) + divx2 * this->grid.get(fx + 1, fy, fz + 1)) +
            divy2 * (divx1 * this->grid.get(fx + 1, fy + 1, fz + 1) + divx2 * this->grid.get(fx, fy + 1, fz + 1))
        );
    }

    template <size_t W, size_t H, size_t V>
    void discrete(matrix3d<T, W, H, V>& mat) {
        if (mat.data == nullptr) return;

        const size_t szx = mat.width();
        const size_t szy = mat.height();
        const size_t szz = mat.volume();

        const T dx = this->p2.x / (szx - 1);
        const T dy = this->p2.y / (szy - 1);
        const T dz = this->p2.z / (szz - 1);
        for (size_t cx = 0; cx < szx; cx++) {
            for (size_t cy = 0; cy < szy; cy++) {
                for (size_t cz = 0; cz < szz; cz++) {
                    mat.get(cx, cy, cz) = interp(cx * dx, cy * dy, cz * dz);
                }
            }
        }
    }
};

}

#endif