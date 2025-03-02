#ifndef LERP2P_HPP
#define LERP2P_HPP

#include "vector.hpp"
#include "constfunc.hpp"

namespace mtp {

/* Interpolation of two points */
template<std::size_t N, std::size_t Points>
struct lerp2p {
    vector<float, N> sp; /* Start Point */
    vector<float, N> ep; /* End Point */
    vector<float, N> control_points[Points]; /* (Control Points) */

    inline void set_point(const std::size_t &index, const vector<float, N> &vec) {
        control_points[index] = vec;
    };

    explicit lerp2p(const vector<float, N> &start_point, const vector<float, N> &end_point) :
        sp(start_point), ep(end_point)
    {

    }

    lerp2p()
    {

    }
};

/**
* @arg N - Space dimension.
*/
template<std::size_t N>
struct quadratic_lerp : public lerp2p<N, 1> {
    using lerp2p<N, 1>::lerp;

    const vector<float, N>& ap = this->control_points[0]; 

    /**
    * @param x_new value within range x_min - x_max
    * @return y_new (interpolated value)
    */
    template<std::size_t n = N, typename std::enable_if_t<n==2, int> = 0>
    inline float interp(const float& x_new) {
        return
            this->sp.y * (((x_new-this->ap.x) * (x_new-this->ep.x)) / ((this->sp.x-this->ap.x) * (this->sp.x-this->ep.x))) +
            this->ap.y * (((x_new-this->sp.x) * (x_new-this->ep.x)) / ((this->ap.x-this->sp.x) * (this->ap.x-this->ep.x))) + 
            this->ep.y * (((x_new-this->sp.x) * (x_new-this->ap.x)) / ((this->ep.x-this->sp.x) * (this->ep.x-this->ap.x))); 
    }

    /**
    * @param x_new value within range x_min - x_max
    * @return vector(y, z);
    */
    template<std::size_t n = N, typename std::enable_if_t<n==3, int> = 0>
    inline vector2f interp(const float& x_new) {
        const float res1 = ((x_new-this->ap.x) * (x_new-this->ep.x)) / ((this->sp.x-this->ap.x) * (this->sp.x-this->ep.x));
        const float res2 = ((x_new-this->sp.x) * (x_new-this->ep.x)) / ((this->ap.x-this->sp.x) * (this->ap.x-this->ep.x));
        const float res3 = ((x_new-this->sp.x) * (x_new-this->ap.x)) / ((this->ep.x-this->sp.x) * (this->ep.x-this->ap.x));
        return {
            this->sp.y * res1 +
            this->ap.y * res2 + 
            this->ep.y * res3,

            this->sp.z * res1 +
            this->ap.z * res2 + 
            this->ep.z * res3
        }; /* vec(y, z) */
    }
    
    using ArrayType = std::conditional_t<N==2, float, vector<float, N-1>>;

    /**
    * @param array pointer to array
    * @param steps
    * @return fills array with interpolated values.
    */
    void interp_range(ArrayType* array, const std::size_t &steps) {
        float dx = (this->ep.x - this->sp.x) / (steps-1);
        for (std::size_t i = 0; i < steps; ++i) array[i] = interp(this->sp.x + i * dx);
    }
};


/**
* @arg Exp - Exponent of bezier curve.
* @arg N - Space dimension.
*/
template <std::size_t Exp, std::size_t N>
struct bezier_curve : public lerp2p<N, Exp-1> {
    using lerp2p<N, Exp-1>::lerp;

    /**
    * @param t value within range 0.0 - 1.0
    * @return y_new
    */
    template <std::size_t E = Exp, typename std::enable_if_t<E == 2 && N==2, int> = 0>
    inline float interp(const float &t) {
        const float dt = (1.0f - t);
        return dt*dt * this->sp.y + 2.0f * t * dt * this->control_points[0].y + t*t * this->ep.y;
    }

    /**
    * @param t value within range 0.0 - 1.0
    * @return vector(y, z) 
    */
    template <std::size_t E = Exp, typename std::enable_if_t<E == 2 && N==3, int> = 0>
    inline vector2f interp(const float &t) {
        const float dt = (1.0f - t);
        const float dt2= dt*dt;
        const float t2 = t*t;
        return {
            dt2 * this->sp.y + 2.0f * t * dt * this->control_points[0].y + t2 * this->ep.y,
            dt2 * this->sp.z + 2.0f * t * dt * this->control_points[0].z + t2 * this->ep.z
        };
    }

    /**
    * @param t value within range 0.0 - 1.0
    * @return y_new
    */
    template <std::size_t B = Exp, typename std::enable_if_t<B == 3 && N == 2, int> = 0>
    inline float interp(const float &t) {
        const float dt = (1.0f - t);
        const float dt2= dt*dt;
        const float t2 = t*t;

        return dt2*dt * this->sp.y + 3.0f * t * dt2 * this->control_points[0].y + 
            3.0f * t2 * dt * this->control_points[1].y + t2*t * this->ep.y;
    }

    /**
    * @param t value within range 0.0 - 1.0
    * @return vector(y, z)
    */
    template <std::size_t B = Exp, typename std::enable_if_t<B == 3 && N == 3, int> = 0>
    inline vector2f interp(const float &t) {
        const float dt = (1.0f - t);
        const float dt2= dt*dt;
        const float dt3= dt*dt;

        const float t2 = t*t;
        const float t3 = t2*t;

        return {
            dt3 * this->sp.y + 3.0f * t * dt2 * this->control_points[0].y + 
                3.0f * t2 * dt * this->control_points[1].y + t3 * this->ep.y,
            
            dt3 * this->sp.z + 3.0f * t * dt2 * this->control_points[0].z + 
                3.0f * t2 * dt * this->control_points[1].z + t3 * this->ep.z  
        };
    }

    using ArrayType = std::conditional_t<N==2, float, vector<float, N-1>>;

    /**
    * @param array pointer to array
    * @param steps
    * @return fills vector with interpolated values.
    */
    void interp_range(ArrayType* array, const std::size_t &steps) {
        float dx = 1.0f/steps;
        for (std::size_t i = 0; i < steps; ++i) array[i] = interp(i * dx);
    }
};

/** 
* @param start 
* @param end
* @param factor value within range 0.0 - 1.0
*/
static constexpr inline float linear_lerp(const float &start, const float &end, const float &factor) {
    return start + (end-start)*factor;
}

/** 
* @param start 
* @param end
* @param factor value within range 0.0 - 1.0
*
* Works for vectors with different size and type.
*/
template <typename T, std::size_t N>
static constexpr inline vector<T, N> linear_lerp(const vector<T, N> &start, const vector<T, N> &end, const float &factor) {
    return start + (end-start)*factor;
}

}

#endif