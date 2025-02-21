#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <cmath>
#include <type_traits>

#include "simd/sse_calc.hpp"
#include "simd/avx_calc.hpp"

template <typename T, std::size_t N, typename Enable = std::enable_if_t<std::is_arithmetic_v<T>>>
class Vector {
private:
    m128_t<T> simd_data;
public:
    static constexpr std::size_t size = N;

    alignas(16) T data[N]; /* The data array can be accessed as a public field for efficient iteration */

    constexpr Vector() : data{} {
        
    }

    constexpr Vector(const m128_t<T> &m_data) : simd_data(m_data), data{} {
        sse::store(data, m_data);
    }

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
    constexpr Vector(Args... args) : data{static_cast<T>(args)...}
    {
        simd_data = sse::load(data);
    }

    /* data access */

    constexpr inline T& operator[](const std::size_t &index) {
        return data[index];
    }

    /* const data access */

    constexpr inline const T& operator[](const std::size_t &index) const {
        return data[index];
    }

    /*  iterator 

        Using of iterators can be not efficient. Use only indexation in a loop if you need more optimization;

    */

    using vector_iterator = T*;
    using vector_citerator = const T*;

    constexpr vector_iterator begin() noexcept { return data; }
    constexpr vector_iterator end() noexcept { return data + N; }

    constexpr vector_citerator begin() const noexcept { return data; }
    constexpr vector_citerator end() const noexcept { return data + N; }

    constexpr vector_citerator cbegin() const noexcept { return data; }
    constexpr vector_citerator cend() const noexcept { return data + N; }

    /* operators */

    constexpr inline Vector operator+(const Vector& other) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }

    constexpr inline Vector operator-(const Vector& other) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    
    constexpr inline Vector operator*(const Vector& other) const {
        if constexpr (N == 4) {
            return Vector(sse::mul<T>(simd_data, other.simd_data));
        } else if constexpr (N != 4) {
            Vector result;
            for (size_t i = 0; i < N; i++) result.data[i] = data[i] * other.data[i];
            return result;
        }
    }

    constexpr inline Vector operator/(const Vector& other) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] / other.data[i];
        return result;
    }

    constexpr inline Vector operator+(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    constexpr inline Vector operator-(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    constexpr inline Vector operator*(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    constexpr inline Vector operator/(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    constexpr inline Vector& operator+=(const Vector& other) {
        for (size_t i = 0; i < N; i++) data[i] += other.data[i];
        return *this;
    }

    constexpr inline Vector& operator-=(const Vector& other) {
        for (size_t i = 0; i < N; i++) data[i] -= other.data[i];
        return *this;
    }

    constexpr inline Vector& operator*=(const Vector& other) {
        for (size_t i = 0; i < N; i++) data[i] *= other.data[i];
        return *this;
    }

    constexpr inline Vector& operator/=(const Vector& other) {
        for (size_t i = 0; i < N; i++) data[i] /= other.data[i];
        return *this;
    }

    constexpr inline Vector& operator+=(T scalar) {
        for (size_t i = 0; i < N; i++) data[i] += scalar;
        return *this;
    }

    constexpr inline Vector& operator-=(T scalar) {
        for (size_t i = 0; i < N; i++) data[i] -= scalar;
        return *this;
    }

    constexpr inline Vector& operator*=(T scalar) {
        for (size_t i = 0; i < N; i++) data[i] *= scalar;
        return *this;
    }

    constexpr inline Vector& operator/=(T scalar) {
        for (size_t i = 0; i < N; i++) data[i] /= scalar;
        return *this;
    }

    /* UTILS methods */

    static inline Vector<T, N> normalize(const Vector<T, N> &vector) {
        Vector<T, N> new_vector;
        double length = 0.0f;

        for(size_t i = 0; i < N; i++) length += vector.data[i]*vector.data[i];
        length = std::sqrt(length);

        for(size_t i = 0; i < N; i++) new_vector.data[i] = vector.data[i]/length;
        return new_vector;
    }

    constexpr inline Vector& normalize() {
        double length = 0.0f;
        for(size_t i = 0; i < N; i++) length += data[i]*data[i];
        for(size_t i = 0; i < N; i++) data[i]/=length;
        return *this;
    }
};

using Vector2i = Vector<int, 2>;
using Vector3i = Vector<int, 3>;
using Vector4i = Vector<int, 4>;

using Vector2u = Vector<unsigned int, 2>;
using Vector3u = Vector<unsigned int, 3>;
using Vector4u = Vector<unsigned int, 4>;

using Vector2f = Vector<float, 2>;
using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;

using Vector2d = Vector<double, 2>;
using Vector3d = Vector<double, 3>;
using Vector4d = Vector<double, 4>;

template <typename T, std::size_t N, std::size_t M, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Matrix {
    const std::size_t size = N*M;
    const std::size_t n = N;
    const std::size_t m = M;

    T data[N*M];
};

using Matrix4x4 = Matrix<float, 4, 4>;
using Matrix4x3 = Matrix<float, 4, 3>;
using Matrix3x3 = Matrix<float, 3, 3>;
using Matrix2x2 = Matrix<float, 2, 2>;

struct Quat {
    float w;
    float x;
    float y;
    float z;
};

#endif