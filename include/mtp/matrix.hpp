#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include "container.hpp"

namespace mtp {

template <typename T = float, std::size_t N = 0, std::size_t M = N>
struct matrix : public DataContainer<T, N*M> {
    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const DataContainer<T, N>& container) 
    {
        std::copy(container.data, container.data+N, this->data);
    }

    /* Dynamics Constructors */
    matrix(const std::size_t &n, const std::size_t &m)
    {
        if constexpr(N==0) {
            this->data = new T[n*m]{};
            this->n = n;
            this->m = m;
        }
    }

    matrix(const std::size_t &n, const std::size_t &m, const T& scalar) : DataContainer<T, 0>(n*m), n(n), m(m) 
    {
        for(std::size_t i = 0; i < this->size; i++) this->data[i] = scalar;
    }

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
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < n; j++) new_vector.data[i]+=vec.data[j]*this->data[i * N + j];
        }
        return new_vector;
    }

    vector<T, N> operator+(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < n; j++) new_vector.data[i]+=vec.data[j]+this->data[i * N + j];
        }
        return new_vector;
    }

    vector<T, N> operator-(const DataContainer<T, N>& vec) {
        vector<T, N> new_vector;
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < n; j++) new_vector.data[i]+=vec.data[j]-this->data[i * N + j];
        }
        return new_vector;
    }

    matrix<T, N, M> operator*(const matrix<T, N, M>& mat) {
        matrix<T, N, M> new_mat(n, m);
        for (size_t i = 0; i < m; i++) {
            size_t index = i*n;
            for (size_t j = 0; j < n; j++) {
                for (size_t k = 0; k < n; k++) {
                    new_mat.data[index+j]+=this->data[index+k] * mat.data[k*n+j];
                }
            }
        }
        return new_mat;
    } /* M!=N - O(M*N^2) | M==N - O(N^3) */

    matrix<T, N, M> operator+(const matrix<T, N*M>& mat) {
        matrix<T, N, M> new_mat(n, m);
        for (size_t i = 0; i < m; i++) {
            size_t index = i*n;
            for (size_t j = 0; j < n; j++) {
                for (size_t k = 0; k < n; k++) {
                    new_mat.data[index+j]+=this->data[index+k] + mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } 

    matrix<T, N, M> operator-(const matrix<T, N*M>& mat) noexcept {
        matrix<T, N, M> new_mat(n, m);
        for (size_t i = 0; i < m; i++) {
            size_t index = i*n;
            for (size_t j = 0; j < n; j++) {
                for (size_t k = 0; k < n; k++) {
                    new_mat.data[index+j]+=this->data[index+k] - mat.data[k*N+j];
                }
            }
        }
        return new_mat;
    } 

    inline const size_t& n_size() const noexcept {return this->n;}
    inline const size_t& m_size() const noexcept {return this->m;}
private:
    size_t n = N;
    size_t m = M;
};

template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, M, N> transpose(const matrix<T, N, M>& mat) {
    const size_t &n = mat.n, &m = mat.m;
    matrix<T, M, N> new_matrix(n, m);
    for(std::size_t i = 0; i < m; i++) {
        for(std::size_t j = 0; j < n; j++) {
            new_matrix.data[j*n+i] = mat.data[i*n+j];
        }
    }
    return new_matrix;
}

template <typename T, std::size_t N, std::size_t M>
constexpr matrix<T, N, M> pow(const matrix<T, N, M>& mat, const std::size_t &exp) {
    const size_t &n = mat.n, &m = mat.m;
    vector<T, N, M> new_mat(n, m);
    for (size_t i = 0; i < new_mat.size; i++) {
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
template<typename T, std::size_t N, std::size_t M = N>
struct matrix_csr : matrix<T, N, M> {
    unsigned int col_indices[N*M] {};
    unsigned int row_ptr[M+1] {};

    unsigned int values_size = 0;

    constexpr matrix_csr() {};

    constexpr matrix_csr(const matrix_csr&) noexcept = default;

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N*M>>
    matrix_csr(const Args&... args)
    { 
        std::size_t i = 0, row = 1;
        row_ptr[0] = 0;
        for(const T& arg : {args...}) {
            if(arg!=0) {
                col_indices[values_size] = i%N;
                values_size++; 
            }
            this->data[i] = arg;
            i++;
            
            if(i%N==0) {
                row_ptr[row] = values_size;
                row++;
            }
        }
    }

    matrix_csr(const T& scalar)
    { 
        if(scalar==0) return;

        std::size_t i = 0, row = 1;
        row_ptr[0] = 0;
        while(i<N*M) {
            col_indices[i] = i%N;
            this->data[i] = scalar;
            i++;
            
            if(i%N==0) {
                row_ptr[row] = i;
                row++;
            }
        }
    }

    matrix_csr(const matrix<T, N, M>& mat)
    { 
        std::size_t i = 0, row = 1;
        row_ptr[0] = 0;
        while(i<N*M) {
            const T& arg = mat.data[i];
            if(arg!=0) {
                col_indices[values_size] = i%N;
                values_size++; 
            }
            this->data[i] = arg;
            i++;
            
            if(i%N==0) {
                row_ptr[row] = values_size;
                row++;
            }
        }
    }

    vector<T, N> operator*(const vector<T, N>& vec) {
        vector<T, N> new_vec;
        for (size_t i = 0; i < M; i++) {
            const size_t row = i*N;
            for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
                const unsigned int &col = col_indices[j];
                new_vec.data[i] += this->data[row+col] * vec.data[col];
            }
        }
        return new_vec;
    }

    matrix_csr<T, N, M> operator*(const matrix<T, N, M>& mat) {
        matrix_csr<T, N, M> new_mat;
        for (size_t i = 0; i < M; i++) {
            const size_t row = i*N;
            for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
                const size_t index = col_indices[k]*N;
                const T& val = this->data[k];
    
                for (size_t j = 0; j < N; j++) {
                    new_mat.data[row+j] += val * mat.data[index+j];
                }
            }
        }
        return new_mat;
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

using matrix4x3 = matrix<float, 4, 3>;
using matrix3x4 = matrix<float, 3, 4>;
using matrix3x2 = matrix<float, 3, 2>;
using matrix2x3 = matrix<float, 2, 3>;

using matrix4f = matrix<float, 4, 4>;
using matrix3f = matrix<float, 3, 3>;
using matrix2f = matrix<float, 2, 2>;

using dmatrix = matrix<>; /* dynamic matrix specialization */

template <typename T> using matrix4 = matrix<T, 4, 4>;
template <typename T> using matrix3 = matrix<T, 3, 3>;
template <typename T> using matrix2 = matrix<T, 2, 2>;

}

#endif