#ifndef LERP2P_HPP
#define LERP2P_HPP

#include "vector.hpp"

namespace mtp {

/** 
* @param start 
* @param end
* @param factor value within range 0.0 - 1.0
*/
static constexpr inline float lerp(const float &start, const float &end, const float &factor) {
    return start + (end-start)*factor;
}

/** 
* @brief Works for vectors with different size and type.
* @param start 
* @param end
* @param factor value within range 0.0 - 1.0
*/
template <typename T, std::size_t N>
static constexpr inline vector<T, N> lerp(const vector<T, N> &start, const vector<T, N> &end, const float &factor) {
    return start + (end-start)*factor;
}

template <std::size_t N, std::size_t AnchorPoints>
struct lerp_data {
    static constexpr std::size_t POINTS_SIZE = AnchorPoints+2; 
    vector<float, N> points[AnchorPoints+2]; /* (All Points) includes start and end point */

    lerp_data(const vector<float, N> &start_point, const vector<float, N> &end_point)
    {
        points[0] = start_point;
        points[AnchorPoints+1] = end_point;
    }

    inline vector<float, N>& first() {return points[0];}
    inline vector<float, N>& last()  {return points[AnchorPoints+1];}
};

/**
* @brief The realization of langrange polynomial. Efficiently interpolates
         values with order 1 - 2. 
* @arg Order - The order of lagrange polynomial.
* @arg N - Space dimension.
*/
template<std::size_t N = 2, std::size_t Order = 1>
struct lagrange_poly : public lerp_data<N, Order-1> {
    float D[Order+1]; /* Denominators */

    lagrange_poly(const vector<float, N> &start_point, const vector<float, N> &end_point) : 
        lerp_data<N, Order-1>(start_point, end_point)
    {
        update_denominators();
    }

    /**
    * @brief Changes a point and updates all denominators of lagrange polynomial.
             Use only this method If you need to mutate one point.
    * @param index
    * @param point vector<float, N>
    */
    void update_point(const std::size_t index, const vector<float, N> &point) {
        this->points[index] = point;
        update_denominators();
    }

    void update_denominators() {
        if constexpr (Order > 2) {
            for (std::size_t i = 0; i < this->POINTS_SIZE; ++i) {
                D[i] = 1.0f;
                for (std::size_t j = 0; j < this->POINTS_SIZE; ++j) {
                    if (i != j) {
                        D[i] *= this->points[i].x - this->points[j].x;
                    }
                }
            }
        }
        if constexpr (Order==2) {
            D[0] = (this->points[0].x-this->points[1].x) * (this->points[0].x-this->points[2].x);
            D[1] = (this->points[1].x-this->points[0].x) * (this->points[1].x-this->points[2].x);
            D[2] = (this->points[2].x-this->points[0].x) * (this->points[2].x-this->points[1].x);
        } 
        else if constexpr (Order==1) {
            D[0] = this->points[0].x-this->points[1].x;
            D[1] = this->points[1].x-this->points[0].x;
        }
    }

    /**
    * @param factor value within range 0.0 - 1.0
    * @return vector<float, N>
    */
    template<std::size_t order = Order, typename std::enable_if_t<order==1, int> = 0>
    inline vector<float, N> interp(const float& factor) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[1].x, factor);
        const float diff1 = x_new-this->points[0].x;
        const float diff2 = x_new-this->points[1].x;
        if constexpr (N==2) {
            return {
                x_new,

                this->points[0].y * diff2 / D[0] +
                this->points[1].y * diff1 / D[1]
            }; /* vector(x, y) */
        } else if constexpr (N==3) {
            const float res1 = diff2 / D[0];
            const float res2 = diff1 / D[1];
            return {
                x_new,

                this->points[0].y * res1 +
                this->points[1].y * res2,

                this->points[0].z * res1 +
                this->points[1].z * res2,
            }; /* vec(x, y, z) */
        }
    }

    /**
    * @param factor value within range 0.0 - 1.0
    * @return vector<float, N>
    */
    template<std::size_t order = Order, typename std::enable_if_t<order==2, int> = 0>
    inline vector<float, N> interp(const float& factor) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[2].x, factor);
        const float diff1 = x_new-this->points[0].x;
        const float diff2 = x_new-this->points[1].x;
        const float diff3 = x_new-this->points[2].x;
        if constexpr (N==2) {
            return {
                x_new,

                this->points[0].y * (diff2 * diff3) / D[0] +
                this->points[1].y * (diff1 * diff3) / D[1] + 
                this->points[2].y * (diff1 * diff2) / D[2]
            }; /* vector(x, y) */
        } else if constexpr (N==3) {
            const float res1 = (diff2 * diff3) / D[0];
            const float res2 = (diff1 * diff3) / D[1];
            const float res3 = (diff1 * diff2) / D[2];
            return {
                x_new,

                this->points[0].y * res1 +
                this->points[1].y * res2 + 
                this->points[2].y * res3,

                this->points[0].z * res1 +
                this->points[1].z * res2 + 
                this->points[2].z * res3
            }; /* vec(x, y, z) */
        }
    }

    /**
    * @param factor value within range 0.0 - 1.0
    * @return vector(x, y) (interpolated vector)
    */
    template<std::size_t n = N, typename std::enable_if_t<n==2 && (Order > 2), int> = 0>
    inline vector2f interp(const float& factor) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[this->points_size-1].x, factor);
        float y_new = 0.0f;
        for (std::size_t i = 0; i < this->points_size; ++i) {
            float L_i = 1.0f, d = 1.0f;
            for (std::size_t j = 0; j < this->points_size; ++j) {
                if (i != j) {
                    L_i *= (x_new - this->points[j].x);
                }
            }
            y_new += this->points[i].y * (L_i / D[i]);
        }
        return {x_new, y_new};
    }

    /**
    * @param factor value within range 0.0 - 1.0
    * @return vector(x, y, z) (interpolated vector)
    */
    template<std::size_t n = N, typename std::enable_if_t<n==3  && (Order > 2), int> = 0>
    vector3f interp(const float& factor) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[this->POINTS_SIZE-1].x, factor);
        float y_new = 0.0f, z_new = 0.0f;
        for (std::size_t i = 0; i < this->POINTS_SIZE; ++i) {
            float L_i = 1.0f;
            for (std::size_t j = 0; j < this->POINTS_SIZE; ++j) {
                if (i != j) {
                    L_i *= x_new - this->points[j].x;
                }
            }
            const float div = (L_i / D[i]);
            y_new += this->points[i].y * div;
            z_new += this->points[i].z * div;
        }
        return {x_new, y_new, z_new};
    }

    /**
    * @param array pointer to array of vectors.
    * @param steps
    * @return fills array with interpolated vectors.
    */
    void interp(vector<float, N>* array, const std::size_t &steps) {
        const float dx = 1.0f / steps;
        for (std::size_t i = 0; i < steps; ++i) array[i] = interp(i * dx);
    }
};

/**
* @brief The realization of bezier curve 2-3 order.
* @arg Order - The order of bezier curve.
* @arg N - Space dimension.
*/
template <std::size_t Order, std::size_t N>
struct bezier_curve : public lerp_data<N, Order-1> {
    static_assert(Order!=1, "The order of bezier must greater than 1");

    using lerp_data<N, Order-1>::lerp_data;

    /**
    * @param t value within range 0.0 - 1.0
    * @return vector<float, N>
    */
    template <std::size_t order = Order, typename std::enable_if_t<order == 2, int> = 0>
    inline vector<float, N> interp(const float &t) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[1].x, t);
        const float dt = (1.0f - t);
        if constexpr (N==2) {
            return {
                x_new,
                dt*dt * this->sp.y + 2.0f * t * dt * this->control_points[0].y + t*t * this->ep.y
            };
        } else if constexpr(N==3) {
            const float dt2 = dt*dt;
            const float t2 = t*t;
            const float mul = 2.0f * t * dt;
            return {
                x_new,
                dt2 * this->sp.y + mul * this->control_points[0].y + t2 * this->ep.y,
                dt2 * this->sp.z + mul * this->control_points[0].z + t2 * this->ep.z
            };
        }
    }

    /**
    * @param t value within range 0.0 - 1.0
    * @return y_new
    */
    template <std::size_t order = Order, typename std::enable_if_t<order == 3, int> = 0>
    inline float interp(const float &t) {
        const float x_new = mtp::lerp(this->points[0].x, this->points[2].x, t);
        const float dt = (1.0f - t);
        const float dt2= dt*dt;
        const float t2 = t*t;
        if constexpr (N==2) {
            return {
                x_new,

                dt2*dt * this->sp.y + 3.0f * t * dt2 * this->control_points[0].y + 
                3.0f * t2 * dt * this->control_points[1].y + t2*t * this->ep.y
            };
        } else if constexpr (N==3) {
            const float dt3  = dt2*dt;
            const float t3   = t2*t;
            const float dtt3 = 3.0f * dt*t;
            const float mul1 = dtt3*dt;
            const float mul2 = dtt3*t;
            return {
                x_new,

                dt3 * this->sp.y + mul1 * this->control_points[0].y + 
                mul2 * this->control_points[1].y + t3 * this->ep.y,

                dt3 * this->sp.z + mul1 * this->control_points[0].z + 
                mul2 * this->control_points[1].z + t3 * this->ep.z
            };
        }  
    }

    /**
    * @param array pointer to array of vectors.
    * @param steps
    * @return fills vector with interpolated values.
    */
    void interp(vector<float, N>* array, const std::size_t &steps) {
        const float dx = 1.0f/steps;
        for (std::size_t i = 0; i < steps; ++i) array[i] = interp(i * dx);
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