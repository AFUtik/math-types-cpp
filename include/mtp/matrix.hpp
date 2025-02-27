#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include "container.hpp"

namespace mtp {

template <typename T, std::size_t N, std::size_t M = N>
struct matrix : public DataContainer<T, N*M> {
    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const matrix&) noexcept = default;

    constexpr matrix(const DataContainer<T, N>& container) {std::copy(container.data, container.data+N, this->data);}

    //constexpr matrix(const DataContainer<T, N*M>& container) {
    //    std::copy(container.data, container.data + this->size, this->data);
    //}

    /**
    * @brief Converts matrix row to vector.
    * @param row index of a row. (starts with zero)
    */
    constexpr inline vector<T*, N> vector_view(const std::size_t &row) {
        vector<T*, N> new_vector;
        for(size_t i = 0; i < N; i++) new_vector[i] = &this->data[row*N+i];
        return new_vector;
    }

    /**
    * @brief Gets an object by xy cordinates.
    * @param x Width
    * @param y Height
    * @return Returns T object.
    */
    constexpr inline T& get(const size_t& x, const size_t& y) {
        return this->data[y*N+x];
    }

    /* vector-matrix multiplication - O(N^2) */
    vector<T, N> operator*(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]*this->data[i * N + j];
        }
        return new_vector;
    } /* M!=N - O(M*N) | M==N - O(N^2) */

    /* classic matrix multiplication */
    matrix<T, N, M> operator*(const DataContainer<T, N*M>& mat) {
        matrix<T, N, M> new_mat;
        for (size_t i = 0; i < M; i++) {
            size_t index = i*N;
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    new_mat.data[index+j]+=this->data[index+k] * mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } /* M!=N - O(M*N^2) | M==N - O(N^3) */

    /* classic matrix sum */
    matrix<T, N, M> operator+(const DataContainer<T, N*M>& mat) {
        matrix<T, N, M> new_mat;
        for (size_t i = 0; i < M; i++) {
            size_t index = i*N;
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    new_mat.data[index+j]+=this->data[index+k] + mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } /* M!=N - O(M*N^2) | M==N - O(N^3) */

    /* classic matrix subtract */
    matrix<T, N, M> operator-(const DataContainer<T, N*M>& mat) {
        vector<T, N, M> new_mat;
        for (size_t i = 0; i < M; i++) {
            size_t index = i*N;
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    new_mat.data[index+j]+=this->data[index+k] - mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } /* M!=N - O(M*N^2) | M==N - O(N^3) */

    /* classic matrix divide */
    matrix<T, N, M> operator/(const DataContainer<T, N*M>& mat) {
        vector<T, N, M> new_mat;
        for (size_t i = 0; i < M; i++) {
            size_t index = i*N;
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    new_mat.data[index+j]+=this->data[index+k] / mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } /* M!=N - O(M*N^2) | M==N - O(N^3) */
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