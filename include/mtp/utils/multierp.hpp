#ifndef MULTIERP_HPP
#define MULTIERP_HPP

#include "lerp.hpp"

namespace mtpu {

template <std::size_t N>
struct blerp_data { /* Bilinear Interpolation Data */
    vector2f p1, p2;

    float Q1, Q2, Q3, Q4;
    bool normalize = false;

    /*                      SCHEME 
    *   (X: 0.0, Y: 1.0) - Q3=====Q4 - p2(X: 1.0, Y: 1.0)
    *                      ||     ||
    *                      ||     ||
    * p1(X: 0.0, Y: 0.0) - Q1=====Q2 - (X: 1.0, Y: 0.0)
    */

    constexpr blerp_data(const float &Q1, const float &Q2,
                         const float &Q3, const float &Q4) :
        Q1(Q1), Q2(Q2), Q3(Q3), Q4(Q4)
    {}
};

template <std::size_t N, std::size_t AnchorPoints>
struct trilerp_data {  /* Trilinear Interpolation Data */
    vector3f p1, p2;

    float Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8;
    bool normalize = false;

    /*                     SCHEME
    *                    Q7------Q8 - p2(X: 1.0, Y: 1.0, Z: 1.0)
    *                     |\      | \
    *  (X: 0.0, Y: 1.0) - | Q3----|--\Q4 - (X: 1.0, Y: 1.0)
    *                     | |     |  |
    *                    Q5------Q6  |
    *                      \|       \|
    *                       \--------\
    *  p1(X: 0.0, Y: 0.0) - Q1        Q2 - (X: 1.0, Y: 0.0)
    */

    constexpr trilerp_data(const float &Q1, const float &Q2,
                           const float &Q3, const float &Q4,
                           const float &Q5, const float &Q6,
                           const float &Q7, const float &Q8) :
    Q1(Q1), Q2(Q2), Q3(Q3), Q4(Q4), Q5(Q5), Q6(Q6), Q7(Q7), Q8(Q8) {}
};

template <typename std::size_t N = 2>
struct blerp : public blerp_data<N> {
    void interp() {

    }
};

}

#endif