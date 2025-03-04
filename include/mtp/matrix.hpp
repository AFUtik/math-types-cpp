#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include "container.hpp"

#include <iostream>

namespace mtp {

template <typename T, std::size_t N, std::size_t M = N>
struct matrix : public DataContainer<T, N*M> {
    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const matrix&) noexcept = default;

    constexpr matrix(const DataContainer<T, N>& container) {std::copy(container.data, container.data+N, this->data);}

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

    vector<T, N> operator*(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]*this->data[i * N + j];
        }
        return new_vector;
    } 

    vector<T, N> operator/(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]/this->data[i * N + j];
        }
        return new_vector;
    }

    vector<T, N> operator+(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]+this->data[i * N + j];
        }
        return new_vector;
    }

    vector<T, N> operator-(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) new_vector.data[i]+=vec.data[j]-this->data[i * N + j];
        }
        return new_vector;
    }

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
    } 

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
    } 

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
    } 

};

template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, M, N> transpose(const matrix<T, N, M>& mat) {
    matrix<T, M, N> new_matrix;
    for(std::size_t i = 0; i < M; i++) {
        for(std::size_t j = 0; j < N; j++) {
            new_matrix.data[j*N+i] = mat.data[i*N+j];
        }
    }
    return new_matrix;
}

template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> pow(const matrix<T, N, M>& mat, const std::size_t &exp) {
    vector<T, N, M> new_mat;
    for (size_t i = 0; i < N*M; i++) {
        T& val = mat.data[i];
        if(val!=0) val = mtp::pow<T>(val, exp);
    }
    return new_mat;
}

/*
template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> det(const matrix<T, N, M>& mat) {}
*/

/*
template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> trace(const matrix<T, N, M>& mat) {}
*/

/*
template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> rref(const matrix<T, N, M>& mat) {}
*/

/*
template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> inverse(const matrix<T, N, M>& mat) {}
*/

/**
* @brief Compressed Sparse Row Matrix.
*/
template<typename T, std::size_t N, std::size_t M>
struct matrix_csr : matrix<T, N, M> {
    using matrix<T, N, M>::matrix;

    unsigned int col_indices[N*M];
    unsigned int row_ptr[M+1];

    unsigned int values_size = 0;

    constexpr matrix_csr(const matrix_csr&) noexcept = default;

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N*M>>
    matrix_csr(const Args&... args)
    { 
        std::size_t i = 0; 
        std::size_t row = 1;
        row_ptr[0] = 0;
        for(const T& arg : {args...}) {
            if(arg!=0) {
                col_indices[values_size] = i%M;
                values_size++; 
            }
            this->data[i] = arg;
            i++;
            
            if(i%M==0) {
                row_ptr[row] = values_size;
                row++;
            }
        }
    }

    vector<T, N> operator*(const vector<T, N>& vec) {
        vector<T, N> new_vec;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
                const unsigned int &col = col_indices[j];
                new_vec.data[i] += this->data[i*N+col] * vec.data[col];
            }
        }
        return new_vec;
    } 
};

template <typename T>
struct dynamic_matrix : DynamicDataContainer<T> {
    std::size_t n = 0;
    std::size_t m = 0;

    using DynamicDataContainer<T>::DynamicDataContainer;

    dynamic_matrix(const std::size_t &rows, const std::size_t &cols) : n(rows), m(cols),
        DynamicDataContainer<T>(rows*cols)
    {

    }

    dynamic_matrix(const std::size_t &rows, const std::size_t &cols, const T& scalar) : n(rows), m(cols),
        DynamicDataContainer<T>(rows*cols, scalar)
    {
        
    }

    /**
    * @brief Gets an object by xy cordinates.
    * @param x Width
    * @param y Height
    * @return Returns T object.
    */
    inline T& get(const size_t& x, const size_t& y) {
        return this->data[y*n+x];
    }

    /* resizes 2-dimensional matrix and resets all values in array to zero */
    void resize(const std::size_t &rows, const std::size_t &cols) {
        n = rows;
        m = cols;

        if(this->data != nullptr) delete[] this->data; 
        this->data = new T[rows*cols]();
    }
};

template <typename T, std::size_t W, std::size_t H = W, std::size_t V = H>
struct matrix3d : public DataContainer<T, W*H*V> {
    static_assert(W!=0 || H!=0 || V!=0, "Matrix size can't be zero.");
    static constexpr size_t WH = W*H; /* The size of the layer of 3d matrix. */

    using DataContainer<T, W*H*V>::DataContainer;

    constexpr matrix3d(const DataContainer<T, W*H*V>& container) {
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

template <typename T>
struct dynamic_matrix3d : DynamicDataContainer<T> {
    std::size_t w = 0;
    std::size_t h = 0;
    std::size_t v = 0;

    using DynamicDataContainer<T>::DynamicDataContainer;

    dynamic_matrix3d(const std::size_t &width, const std::size_t &height, const std::size_t &volume) : 
        w(width), h(height), v(volume), DynamicDataContainer<T>(width*height*volume)
    {

    }

    dynamic_matrix3d(const std::size_t &width, const std::size_t &height, const std::size_t &volume, const T& scalar) : 
        w(width), h(height), v(volume), DynamicDataContainer<T>(width*height*volume, scalar)
    {
        
    }

    /**
    * @brief Gets an object by xyz cordinates.
    * @param x Width
    * @param y Height
    * @param z Volume
    */
    constexpr inline const T& get(const size_t& x, const size_t& y, const size_t& z) {
        return this->data[y*w*h+z*w+x];
    }

    /* resizes 3-dimensional matrix and resets all values in array to zero */
    void resize(const std::size_t &width, const std::size_t &height, const std::size_t &volume) {
        this->w = width;
        this->h = height;
        this->v = volume;

        if(this->data != nullptr) delete[] this->data; 
        this->data = new T[width*height*volume]();
    }
};


template <typename T>
dynamic_matrix<T> transpose(const dynamic_matrix<T>& mat) {
    dynamic_matrix<T> new_matrix(mat.m, mat.n);
    for(std::size_t i = 0; i < mat.m; i++) {
        for(std::size_t j = 0; j < mat.n; j++) {
            new_matrix.data[j*mat.n+i] = mat.data[i*mat.n+j];
        }
    }
    return new_matrix;
}

using matrix4x3 = matrix<float, 4, 3>;
using matrix3x4 = matrix<float, 3, 4>;
using matrix3x2 = matrix<float, 3, 2>;
using matrix2x3 = matrix<float, 2, 3>;

using matrix4f = matrix<float, 4, 4>;
using matrix3f = matrix<float, 3, 3>;
using matrix2f = matrix<float, 2, 2>;

template <typename T> using matrix4 = matrix<T, 4, 4>;
template <typename T> using matrix3 = matrix<T, 3, 3>;
template <typename T> using matrix2 = matrix<T, 2, 2>;

}

#endif