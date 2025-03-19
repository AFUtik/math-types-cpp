#ifndef MULTIERP_HPP
#define MULTIERP_HPP

#include "lerp.hpp"
#include "../matrix.hpp"

namespace mtpu {

template <typename T = float>
struct blerp_data { /* Bilinear Interpolation Data */
    vector<T, 2> p1, p2;

    T Q1, Q2, Q3, Q4;
    bool normalize = false;

    /*                      SCHEME 
    *   (X: 0.0, Y: 1.0) - Q3=====Q4 - p2(X: 1.0, Y: 1.0)
    *                      ||     ||
    *                      ||     ||
    * p1(X: 0.0, Y: 0.0) - Q1=====Q2 - (X: 1.0, Y: 0.0)
    */

    constexpr blerp_data(const T &Q1, const T &Q2,
                         const T &Q3, const T &Q4) :
        Q1(Q1), Q2(Q2), Q3(Q3), Q4(Q4)
    {}
};

template <typename T = float>
struct trilerp_data {  /* Trilinear Interpolation Data */
    vector<T, 3> p1, p2;

    T Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8;
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

    constexpr trilerp_data(const T &Q1, const T &Q2,
                           const T &Q3, const T &Q4,
                           const T &Q5, const T &Q6,
                           const T &Q7, const T &Q8) :
    Q1(Q1), Q2(Q2), Q3(Q3), Q4(Q4), Q5(Q5), Q6(Q6), Q7(Q7), Q8(Q8) {}
};

template <typename T = float>
struct blerp : public blerp_data<T> {
    using blerp_data<T>::blerp_data;

    T interp(const vector<T, 2> &vec) {

    }
};

template <typename T = float>
struct tlerp : public blerp_data<T> {
    T interp(const vector<T, 3> &vec) {
        
    }
};

}

#endif