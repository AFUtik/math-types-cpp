#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include "container.hpp"

namespace mtp {

template <typename T, std::size_t N, std::size_t M>
struct matrix : public DataContainer<T, N*M> {
    static_assert(N!=0 || M!=0, "Matrix size can't be zero.");

    static constexpr std::size_t n = N;
    static constexpr std::size_t m = M;

    // vector<T*, N> rows[M];

    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const DataContainer<T, N*M>& container) {
        std::copy(container.data, container.data + this->SIZE, this->data);
    }

    /* UTILS methods */

    template<typename CastType>
    static constexpr inline matrix<CastType, N, M> cast(const matrix<T, N, M> &container) {
        matrix<CastType, N, M> new_container;
        for(std::size_t i = 0; i < N; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
        return new_container;
    }

    template<std::size_t n, std::size_t m>
    static constexpr inline matrix<T, n, m> resize(const matrix<T, N, M> &mat) {
        matrix<T, n, m> new_container;
        
        for(std::size_t i = 0; i < std::min(n * m, N * M); i++) new_container.data[i] = mat.data[i];
        return new_container;
    }

};

using matrix4x3 = matrix<float, 4, 3>;
using matrix3x4 = matrix<float, 3, 4>;
using matrix3x2 = matrix<float, 3, 2>;
using matrix2x3 = matrix<float, 2, 3>;

using matrix4d = matrix<double, 4, 4>;
using matrix3d = matrix<double, 3, 3>;
using matrix2d = matrix<double, 2, 2>;
using matrix4f = matrix<float, 4, 4>;
using matrix3f = matrix<float, 3, 3>;
using matrix2f = matrix<float, 2, 2>;

template <typename T> using matrix4 = matrix<T, 4, 4>;
template <typename T> using matrix3 = matrix<T, 3, 3>;
template <typename T> using matrix2 = matrix<T, 2, 2>;

}

#endif