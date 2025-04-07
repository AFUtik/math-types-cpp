/**
* @file constfunc.hpp
* @brief MathType Utility File. 
* @author AFUtik
* @date 2025-07-04
* @version v1.0.0-dev
* @copyright MIT
*
* @details Provides Math Functions and constants.
*/

#ifndef CONSTFUNC_HPP
#define CONSTFUNC_HPP

#define PI 3.141592653589793
#define EXP 2.71828182845904

#include <limits>

/* precomputed factorials */
inline std::size_t fact_table[21] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800,
    479001600, 6227020800, 87178291200, 1307674368000, 20922789888000,
    355687428096000, 6402373705728000, 121645100408832000, 2432902008176640000
};

namespace mtpu {

/* exponentiation by squaring */
template <typename T>
constexpr T pow(T base, std::size_t exp) {
    T result = 1;
    while(exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

constexpr double log(double x, int terms = 10) {
    x = (x - 1) / (x + 1);
    double sum = 0;
    for (int n = 0; n < terms; n++) {
        sum += pow(x, 2 * n + 1) / (2 * n + 1);
    }
    return 2 * sum;
}

constexpr double exp(double x, int terms = 10) {
    double sum = 1.0;
    double term = 1.0;
    for (int i = 1; i < terms; i++) {
        term *= x / i;
        sum += term;
    }
    return sum;
}

constexpr double pow(double base, double exponent) {
    if (base == 0) return 0;
    if (exponent == 0) return 1;

    double int_part = static_cast<long long>(exponent);
    double frac_part = exponent - int_part;

    double int_result = pow<double>(base, static_cast<long long>(int_part));
    double frac_result = exp(frac_part * log(base));

    return int_result * frac_result;
}

constexpr inline std::size_t pow10(std::size_t p) {
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

constexpr inline std::size_t factorial(std::size_t n) {
    unsigned long long result = 1;
    while (n > 1) result *= n--;
    return result;
}

/* Returns precomputed factorial in range 0 - 20. */
constexpr inline const std::size_t& factorial_precomputed(std::size_t n) {
    return fact_table[n];
}

//constexpr std::size_t factorial_stirling(const size_t &n) {
//    if (n == 0) return 1;
//    return sqrt(2 * PI * n) * pow(n / EXP, n);
//}

}
    
#endif