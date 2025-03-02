#ifndef LERP_HPP
#define LERP_HPP

#include "vector.hpp"
#include <vector>

namespace mtp {

/* Interpolation of two points */
struct lerp {
    vector2f sp;
    vector2f ep;

    explicit lerp(const vector2f& start_point, const vector2f& end_point) :
        sp(start_point), ep(end_point)
    {

    }

    lerp(float x1, float y1, float x2, float y2) : 
        sp(x1, y1), ep(x2, y2)
    {

    }

    lerp()
    {

    }
};

struct quadratic_lerp : public lerp {
    using lerp::lerp;

    vector2f ap; /* (additional point) for interpolation */

    /**
    * @param x_new 
    * @return y_new (interpolated value)
    */
    template<typename T>
    inline float interp(const T& x_new) {
        return
            sp.y * (((x_new-ap.x) * (x_new-ep.x)) / ((sp.x-ap.x) * (sp.x-ep.x))) +
            ap.y * (((x_new-sp.x) * (x_new-ep.x)) / ((ap.x-sp.x) * (ap.x-ep.x))) + 
            ep.y * (((x_new-sp.x) * (x_new-ap.x)) / ((ep.x-sp.x) * (ep.x-ap.x))); 
    }

    /**
    * @param std_vector
    * @param steps
    * @return fills vector with interpolated values.
    */
    template<typename T>
    void interp_range(T* array, const std::size_t &steps) {
        float dx = (ep.x - sp.x) / (steps-1);
        for (std::size_t i = 0; i < steps; ++i) array[i] = interp(sp.x + i * dx);
    }
};

struct bezier_curve : public lerp {
    using lerp::lerp;

    vector2f ap; /* (additional point) for interpolation */

    /**
    * @param t value within range 0.0 - 1.0
    * @return y_new (interpolated value)
    */
    inline float interp(const float &t) {
        const float dt = (1.0f - t);
        return dt*dt * sp.y + 2.0f * t * dt * ap.y + t*t * ep.y;
    }

    /**
    * @param std_vector
    * @param steps
    * @return fills vector with interpolated values.
    */
    template<typename T>
    void interp_range(T* array, const std::size_t &steps) {
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