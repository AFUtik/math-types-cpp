/**
* @file random.hpp
* @brief MathType Utility File.
* @author AFUtik
* @date 2025-07-04
* @version v1.0.0-dev
* @copyright MIT
* 
* @details Generates random values in DataCointainer class. Can be applied to vector, matrix.
*/

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../container.hpp"

#include <limits>
#include <random>

using namespace mtp;

namespace mtpu {

inline std::random_device rd;
inline std::mt19937 gen(rd());

template <typename T, size_t Size>
void random(DataContainer<T, Size> &container, 
            const T& min = 0, 
            const T& max = 1) 
{
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(min, max);
        for (size_t i = 0; i < container.size(); ++i) {
            container[i] = dist(gen);
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(min, max);
        for (size_t i = 0; i < container.size(); ++i) {
            container[i] = dist(gen);
        }
    } else {
        static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type!");
    }
}

}

#endif 