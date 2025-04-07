/**
* @file lerp.hpp
* @brief MathType Utility File.
* @author AFUtik
* @date 2025-07-04
* @version v1.0.0-dev
* @copyright MIT
* 
* @details Provides different types of linear intepolation.
*/

#ifndef LERP_HPP
#define LERP_HPP

#include "../vector.hpp"

using namespace mtp;

namespace mtpu {

/**
* @param start
* @param end
* @param factor value within range 0.0 - 1.0
*/
static constexpr inline float lerp(const float& start, const float& end, const float& factor) {
    return start + (end - start) * factor;
}

/**
* @brief Works for vectors with different size and type.
* @param start
* @param end
* @param factor value within range 0.0 - 1.0
*/
template <typename T, std::size_t N>
static constexpr inline vector<T, N> lerp(const vector<T, N>& start, const vector<T, N>& end, const float& factor) {
    return start + (end - start) * factor;
}

/*
* Interpolation functions.
*/
template <typename T>
struct interp_funcs {
    /**
    * @brief Interpolates Y
    * @param factor 0.0 - 1.0
    */
    virtual T interpy(float factor) = 0;

    /**
    * @brief Interpolates Z.
    * Works only for 3 Dimension.
    *
    * @param factor 0.0 - 1.0
    */
    virtual T interpz(float factor) = 0;

    /**
    * @brief Interpolates Y and converts to vector2 with X coordinate
    *
    * @param factor 0.0 - 1.0
    */
    virtual vector<T, 2> interp2v(float factor) = 0;

    /**
    * @brief Interpolates Y and Z, after converts to vector3 with X coordinate.
    * Works only for 3 Dimension.
    *
    * @param factor 0.0 - 1.0
    */
    virtual vector<T, 3> interp3v(float factor) = 0;
};

/*
* Linear Interpolation data. Contains all points of interpolation.
* Can only store the predefined amount of points in the template.
* 
* @tparam N - Space dimension.
* @tparam AnchorPoints - The number of anchor points.
* @tparam T - 
*/
template <typename T, size_t N, size_t Order>
struct lerp_data : public interp_funcs<T> {
    static constexpr size_t points_size = Order + 1;
    vector<T, N> points[points_size]; /* (All Points) includes start and end point */

    lerp_data(const vector<T, N>& start_point, const vector<T, N>& end_point)
    {
        points[0] = start_point;
        points[Order] = end_point;
    }

    lerp_data()
    {
        points[0] = vector<T, N>(0);
        points[Order] = vector<T, N>(1);
    }

    inline vector<T, N>& first() { return points[0]; }
    inline vector<T, N>& last()  { return points[Order]; }
};

/*
* Dynamic Linear Interpolation data. Contains all points of interpolation.
* The number of points can be modified.
*/
template <typename T, size_t N>
struct lerp_data<T, N, 0> : public interp_funcs<T> {
    vector<T, N>* points = nullptr;
    size_t points_size = 2;

    lerp_data(const vector<T, N>& start_point, const vector<T, N>& end_point) : points(new T[2])
    {
        points[0] = start_point;
        points[1] = end_point;
    }

    lerp_data(const vector<T, N>& start_point, const vector<T, N>& end_point, size_t anchor_points) : points(new T[anchor_points+2])
    {
        points_size = anchor_points + 2;
        points[0] = start_point;
        points[1+anchor_points] = end_point;
    }

    lerp_data() : points(new T[2])
    {
        points[0] = vector<T, N>(0);
        points[1] = vector<T, N>(1);
    }

    inline vector<T, N>& first() { return points[0]; }
    inline vector<T, N>& last()  { return points[points_size - 1]; }

    /* Mutate Functions */

    void add(const vector<T, N> &vec) {}
    void insert(const vector<T, N>& vec, size_t index) {}

    void remove(size_t index) {}
    void pop_back() {}
    void pop_front() {}

    void resize(size_t new_size) {}
};

template <typename T, size_t Order>
vector<T, 2>& min(lerp_data<T, 2, Order>& ldata) {
    vector<T, 2>& min = ldata.points[0];
    for (size_t i = 1; i < ldata.points_size; i++) {
        if (ldata.points[i].y < min.y) {
            min = ldata.points[i];
        }
    }
    return min;
}

template <typename T, size_t Order>
vector<T, 2>& max(lerp_data<T, 2, Order>& ldata) {
    vector<T, 2>& max = ldata.points[0];
    for (size_t i = 1; i < ldata.points_size; i++) {
        if (ldata.points[i].y > max.y) {
            max = ldata.points[i];
        }
    }
    return max;
}

/**
* @brief The realization of langrange polynomial. Efficiently interpolates
         values with order 2.
* @tparam Order - The order of lagrange polynomial.
* @tparam N - Space dimension.
*/
template <
    typename T = float,
    std::size_t N = 2,
    std::size_t Order = 2
>
struct lagrange_poly : public lerp_data<N, Order, T> {
    static_assert(Order != 1, "The order of lagrange poly must greater than 1");
    T D[Order + 1]; /* Denominators */

    lagrange_poly(const vector<T, N>& start_point, const vector<T, N>& end_point) : lerp_data<T, Order, T>(start_point, end_point)
    {
        update_denominators();
    }

    /**
    * @brief Changes a point and updates all denominators of lagrange polynomial.
             Use only this method If you need to mutate one point.
    * @param index
    * @param point vector<float, N>
    */
    void update_point(const std::size_t index, const vector<T, N>& point) {
        this->points[index] = point;
        update_denominators();
    }

    void update_denominators() {
        if constexpr (Order > 2 || Order == 0) {
            for (std::size_t i = 0; i < this->points_size; ++i) {
                D[i] = 1.0f;
                for (std::size_t j = 0; j < this->points_size; ++j) {
                    if (i != j) {
                        D[i] *= this->points[i].x - this->points[j].x;
                    }
                }
            }
        }
        if constexpr (Order == 2) {
            D[0] = (this->points[0].x - this->points[1].x) * (this->points[0].x - this->points[2].x);
            D[1] = (this->points[1].x - this->points[0].x) * (this->points[1].x - this->points[2].x);
            D[2] = (this->points[2].x - this->points[0].x) * (this->points[2].x - this->points[1].x);
        }
    }

    T interpy(float factor) override {
        if constexpr (Order > 2 || Order == 0) {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[this->points_size - 1].x, factor);
            T y_new = 0.0f;
            for (std::size_t i = 0; i < this->points_size; ++i) {
                T L_i = 1.0f, d = 1.0f;
                for (std::size_t j = 0; j < this->points_size; ++j) {
                    if (i != j) {
                        L_i *= (x_new - this->points[j].x);
                    }
                }
                y_new += this->points[i].y * (L_i / D[i]);
            }
            return y_new;
        }
        else {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[Order].x, factor);
            const T diff1 = x_new - this->points[0].x;
            const T diff2 = x_new - this->points[1].x;
            if constexpr (Order == 2) {
                const T diff3 = x_new - this->points[2].x;
                return this->points[0].y * (diff2 * diff3) / D[0] +
                       this->points[1].y * (diff1 * diff3) / D[1] +
                       this->points[2].y * (diff1 * diff2) / D[2];
            }
        }
    }

    T interpz(float factor) override {
        if constexpr (Order > 2 || Order == 0) {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[this->points_size - 1].x, factor);
            T z_new = 0.0f;
            for (std::size_t i = 0; i < this->points_size; ++i) {
                T L_i = 1.0f, d = 1.0f;
                for (std::size_t j = 0; j < this->points_size; ++j) {
                    if (i != j) {
                        L_i *= (x_new - this->points[j].x);
                    }
                }
                z_new += this->points[i].z * (L_i / D[i]);
            }
            return z_new;
        }
        else {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[Order].x, factor);
            const T diff1 = x_new - this->points[0].x;
            const T diff2 = x_new - this->points[1].x;
            if constexpr (Order == 2) {
                const T diff3 = x_new - this->points[2].x;
                return this->points[0].z * (diff2 * diff3) / D[0] +
                       this->points[1].z * (diff1 * diff3) / D[1] +
                       this->points[2].z * (diff1 * diff2) / D[2];
            }
        }
    }

    vector<T, 2> interp2v(float factor) override {
        if constexpr (Order > 2 || Order == 0) {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[this->points_size - 1].x, factor);
            T y_new = 0.0f;
            for (std::size_t i = 0; i < this->points_size; ++i) {
                T L_i = 1.0f, d = 1.0f;
                for (std::size_t j = 0; j < this->points_size; ++j) {
                    if (i != j) {
                        L_i *= (x_new - this->points[j].x);
                    }
                }
                y_new += this->points[i].y * (L_i / D[i]);
            }
            return { x_new, y_new };
        }
        else {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[Order].x, factor);
            const T diff1 = x_new - this->points[0].x;
            const T diff2 = x_new - this->points[1].x;
            if constexpr (Order == 2) {
                const T diff3 = x_new - this->points[2].x;
                return {
                    x_new,

                    this->points[0].y * (diff2 * diff3) / D[0] +
                    this->points[1].y * (diff1 * diff3) / D[1] +
                    this->points[2].y * (diff1 * diff2) / D[2]
                };
            }
        }
    }

    vector<T, 3> interp3v(float factor) override {
        if constexpr (Order > 2 || Order == 0) {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[this->points_size - 1].x, factor);
            T y_new = 0.0f, z_new = 0.0f;
            for (std::size_t i = 0; i < this->POINTS_SIZE; ++i) {
                T L_i = 1.0f;
                for (std::size_t j = 0; j < this->POINTS_SIZE; ++j) {
                    if (i != j) {
                        L_i *= x_new - this->points[j].x;
                    }
                }
                const T div = (L_i / D[i]);
                y_new += this->points[i].y * div;
                z_new += this->points[i].z * div;
            }
            return { x_new, y_new, z_new };
        }
        else {
            const T x_new = mtpu::lerp(this->points[0].x, this->points[Order].x, factor);
            const T diff1 = x_new - this->points[0].x;
            const T diff2 = x_new - this->points[1].x;
            if constexpr (Order == 2) {
                const T diff3 = x_new - this->points[2].x;
                const T res1 = (diff2 * diff3) / D[0];
                const T res2 = (diff1 * diff3) / D[1];
                const T res3 = (diff1 * diff2) / D[2];
                return {
                    x_new,

                    this->points[0].y * res1 +
                    this->points[1].y * res2 +
                    this->points[2].y * res3,

                    this->points[0].z * res1 +
                    this->points[1].z * res2 +
                    this->points[2].z * res3
                };
            }
        }
    }
};

/**
* @brief The realization of bezier curve 2-3 order.
* @tparam Order - The order of bezier curve.
* @tparam N - Space dimension.
*/
template <
    typename T = float,
    std::size_t N = 2, 
    std::size_t Order = 2
>
struct bezier_curve : public lerp_data<T, N, Order> {
    static_assert(Order != 1, "The order of bezier must greater than 1");

    using DataType::DataType;

    T interpy(float t) override {
        const T x_new = mtpu::lerp(this->points[0].x, this->points[1].x, t);
        const T dt = (1.0f - t);
        if constexpr (Order == 2) {
            return dt * dt * this->points[0].y + 2.0f * t * dt * this->points[1].y + t * t * this->points[2].y;
        }
        else if constexpr (Order == 3) {
            const T dt2 = dt * dt;
            const T t2 = t * t;
            return dt2 * dt * this->points[0].y + 3.0f * t * dt2 * this->points[1].y +
                   3.0f * t2 * dt * this->points[2].y + t2 * t * this->points[3].y;
        };
    }
    T interpz(float t) override {
        const T x_new = mtpu::lerp(this->points[0].x, this->points[1].x, t);
        const T dt = (1.0f - t);
        if constexpr (Order == 2) {
            return dt * dt * this->points[0].z + 2.0f * t * dt * this->points[1].z + t * t * this->points[2].z;
        }
        else if constexpr (Order == 3) {
            const T dt2 = dt * dt;
            const T t2 = t * t;
            return dt2 * dt * this->points[0].z + 3.0f * t * dt2 * this->points[1].z +
                   3.0f * t2 * dt * this->points[2].z + t2 * t * this->points[3].z;
        };
    }
    vector<T, 2> interp2v(float t) override {
        const T x_new = mtpu::lerp(this->points[0].x, this->points[1].x, t);
        const T dt = (1.0f - t);
        if constexpr (Order == 2) {
            return { 
                x_new, 

                dt * dt * this->points[0].y + 2.0f * t * dt * this->points[1].y + t * t * this->points[2].y 
            };
        }
        else if constexpr (Order == 3) {
            const T dt2 = dt * dt;
            const T t2 = t * t;
            return {
                x_new,

                dt2 * dt * this->points[0].y + 3.0f * t * dt2 * this->points[1].y +
                3.0f * t2 * dt * this->points[2].y + t2 * t * this->points[3].y
            };
        }
    }
    vector<T, 3> interp3v(float t) override {
        const T x_new = mtpu::lerp(this->points[0].x, this->points[1].x, t);
        const T dt = (1.0f - t);
        const T dt2 = dt * dt;
        const T t2 = t * t;
        const T mul = 2.0f * t * dt;
        if constexpr (Order == 2) {
            return {
                x_new,

                dt2* this->points[0].y + mul * this->points[1].y + t2 * this->points[2].y,
                dt2* this->points[0].z + mul * this->points[1].z + t2 * this->points[2].z
            };
        }
        else if constexpr (Order == 3) {
            const T dt3 = dt2 * dt;
            const T t3 = t2 * t;
            const T dtt3 = 3.0f * dt * t;
            const T mul1 = dtt3 * dt;
            const T mul2 = dtt3 * t;
            return {
                x_new,

                dt3 * this->points[0].y + mul1 * this->points[1].y +
                mul2 * this->points[2].y + t3 * this->points[3].y,

                dt3 * this->points[0].z + mul1 * this->points[1].z +
                mul2 * this->points[2].z + t3 * this->points[3].z
            };
        }
    }
};

/* splines */
    // struct cubic_spline {};

    // struct bspline {};

/* polynomial */
    // struct newton_poly {};

/* on basis func */
    // struct gaussian_lerp {};

    // struct rbf_lerp {}; (radical basis functions)

/* filters */
    // struct lanczos_lerp {};

    // struct sinc_lerp {};

}

#endif