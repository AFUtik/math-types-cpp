#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include "container.hpp"

namespace mtp {

template <typename T, std::size_t N, std::size_t M = N>
struct matrix : public DataContainer<T, N*M> {
    static_assert(N!=0 || M!=0, "Matrix size can't be zero.");

    //static constexpr std::size_t n = N;
    //static constexpr std::size_t m = M;

    // vector<T*, N> rows[M];

    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const DataContainer<T, N*M>& container) {
        std::copy(container.data, container.data + this->size, this->data);
    }

    /**
    * @brief Gets an object by xy cordinates.
    * @param x Width
    * @param y Height
    */
    constexpr inline const T& get(const size_t& x, const size_t& y) {
        return this->data[y*N+x];
    }

    /** 
    * @brief The product of matrix and vector.
    * @return Returns a multiplied vector by matrix.
    */
    constexpr inline vector<T, N> operator*(const vector<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]*this->data[i * N + j];
        }
        return new_vector;
    }
};


template <typename T, std::size_t W, std::size_t H = W, std::size_t V = H>
struct matrix3D : public DataContainer<T, W*H*V> {
    static_assert(W!=0 || H!=0 || V!=0, "Matrix size can't be zero.");
    static constexpr size_t WH = W*H; /* The size of the layer of 3d matrix. */

    using DataContainer<T, W*H*V>::DataContainer;

    constexpr matrix3D(const DataContainer<T, W*H*V>& container) {
        std::copy(container.data, container.data + this->size, this->data);
    }

    /**
    * @brief Gets an object by xyz cordinates.
    * @param x Width
    * @param y Height
    * @param z Volume
    */
    constexpr inline const T& get(const size_t& x, const size_t& y, const size_t& z) {
        static_assert(x<=W || y<=H || z<=V, "The cordinates exceed the limits of 3d matrix.");

        return this->data[y*WH+z*W+x];
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