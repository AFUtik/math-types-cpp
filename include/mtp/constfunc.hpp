#ifndef CONSTFUNC_HPP
#define CONSTFUNC_HPP

#include <limits>

namespace mtp {

constexpr inline size_t pow10(size_t p) {
    return (p == 0) ? 1 : 10 * pow10(p - 1);
}

template<typename T = float>
T constexpr inline sqrtNewtonRaphson(const T &x, const T &curr, const T &prev)
{
    return curr == prev
        ? curr
        : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}

template<typename T = float>
T constexpr inline sqrt(const T &x)
{
    return x >= 0 && x < std::numeric_limits<T>::infinity()
        ? sqrtNewtonRaphson<T>(x, x, 0)
        : std::numeric_limits<T>::quiet_NaN();
}

}
    
#endif