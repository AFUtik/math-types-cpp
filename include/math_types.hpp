#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <type_traits>
#include <algorithm>

#define INIT_XYZW_RGBA \
    x(this->data[0]), \
    y(N>1 ? this->data[1] : this->data[0]), \
    z(N>2 ? this->data[2] : this->data[0]), \
    w(N>3 ? this->data[3] : this->data[0]), \
    r(this->data[0]), \
    g(N>1 ? this->data[1] : this->data[0]), \
    b(N>2 ? this->data[2] : this->data[0]), \
    a(N>3 ? this->data[3] : this->data[0]) \

#define POW10(p, result) ((p == 0) ? result : power10(p - 1, result * 10))

/* Namespace Math Type*/
namespace mtp {

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DataContainer {
    static constexpr float epsilon = 1.0f / static_cast<float>(POW10(Precition, 1));
    T data[Size];
    
    constexpr DataContainer() : data{} {}

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == Size>>
    constexpr DataContainer(Args... args) : data{static_cast<T>(args)...} {}

    constexpr DataContainer(const T& scalar)
    {
        for(std::size_t i = 0; i < Size; i++) data[i] = scalar;
    }

    using data_iterator = T*;
    using data_citerator = const T*;

    constexpr data_iterator begin() noexcept { return data; }
    constexpr data_iterator end() noexcept { return data + Size; }

    constexpr data_citerator begin() const noexcept { return data; }
    constexpr data_citerator end() const noexcept { return data + Size; }

    constexpr data_citerator cbegin() const noexcept { return data; }
    constexpr data_citerator cend() const noexcept { return data + Size; }

    constexpr inline T& operator[](const std::size_t &index) {return data[index];}

    constexpr inline const T& operator[](const std::size_t &index) const {return data[index];}

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() + std::declval<U>()), U>>
    constexpr inline DataContainer operator+(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() - std::declval<U>()), U>>
    constexpr inline DataContainer operator-(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    
    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() * std::declval<U>()), U>>
    constexpr inline DataContainer operator*(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * other.data[i];
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() / std::declval<U>()), U>>
    constexpr inline DataContainer operator/(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / other.data[i];
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() + std::declval<U>()), U>>
    constexpr inline DataContainer operator+(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() - std::declval<U>()), U>>
    constexpr inline DataContainer operator-(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() * std::declval<U>()), U>>
    constexpr inline DataContainer operator*(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() / std::declval<U>()), U>>
    constexpr inline DataContainer operator/(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() += std::declval<U>()), U>>
    constexpr inline DataContainer& operator+=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] += other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() -= std::declval<U>()), U>>
    constexpr inline DataContainer& operator-=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] -= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() *= std::declval<U>()), U>>
    constexpr inline DataContainer& operator*=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] *= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() /= std::declval<U>()), U>>
    constexpr inline DataContainer& operator/=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] /= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() += std::declval<U>()), U>>
    constexpr inline DataContainer& operator+=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] += scalar;
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() -= std::declval<U>()), U>>
    constexpr inline DataContainer& operator-=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] -= scalar;
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() *= std::declval<U>()), U>>
    constexpr inline DataContainer& operator*=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] *= scalar;
        return *this;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() /= std::declval<U>()), U>>
    constexpr inline DataContainer& operator/=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] /= scalar;
        return *this;
    }

    /* Comparison operators. Returns only bitmask */

    template<typename U = T, typename = std::void_t<decltype(std::declval<U>() > std::declval<U>()), U>>
    constexpr inline int operator>(const DataContainer& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] > other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() < std::declval<U>()), U>>
    constexpr inline int operator<(const DataContainer& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] < other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() == std::declval<U>()), U>>
    constexpr inline int operator==(const DataContainer& other) {
        if constexpr(std::is_floating_point_v<T>) {
            int bitmask = 0;
            for (size_t i = 0; i < Size; i++) {
                bitmask<<=1;
                if(data[i] - other.data[i] < epsilon) bitmask|=1;
            }
            return bitmask;
        } else {
            int bitmask = 0;
            for (size_t i = 0; i < Size; i++) {
                bitmask<<=1;
                if(data[i] == other.data[i]) bitmask|=1;
            }
            return bitmask;
        }
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() != std::declval<U>()), U>>
    constexpr inline int operator!=(const DataContainer& other) {
        if constexpr(std::is_floating_point_v<T>) {
            int bitmask = 0;
            for (size_t i = 0; i < Size; i++) {
                bitmask<<=1;
                if(data[i] - other.data[i] > epsilon) bitmask|=1;
            }
            return bitmask;
        } else {
            int bitmask = 0;
            for (size_t i = 0; i < Size; i++) {
                bitmask<<=1;
                if(data[i] != other.data[i]) bitmask|=1;
            }
            return bitmask;
        }
    }
};

template <typename T, std::size_t N>
struct vector : public DataContainer<T, N> {
    static_assert(N!=0, "Vector size can't be zero.");

    static constexpr std::size_t size = N;
    T &x, &y, &z, &w; /* XYZW */
    T &r, &g, &b, &a; /* RGBA */

    constexpr vector() : DataContainer<T, N>(), INIT_XYZW_RGBA {}

    constexpr vector(T scalar) : DataContainer<T, N>(scalar), INIT_XYZW_RGBA {}

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
    constexpr vector(Args... args) : DataContainer<T, N>(args...), INIT_XYZW_RGBA {}

    /* Data assigning */

    constexpr vector(const DataContainer<T, N>& container) : INIT_XYZW_RGBA 
    {
        std::copy(container.data, container.data + size, this->data);
    }

    constexpr inline void operator=(const vector<T, N>& container) {
        x = container.x; r = container.r;
        y = container.y; g = container.g;
        z = container.z; b = container.b;
        w = container.w; a = container.a;
    }

    /* UTILS methods */

    template<typename CastType>
    static constexpr inline vector<CastType, N> cast(const vector<T, N> &container) {
        vector<CastType, N> new_container;
        for(std::size_t i = 0; i < N; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
        return new_container;
    }

    constexpr inline vector& normalize() {
        double length = 0.0f;
        for(size_t i = 0; i < N; i++) length += this->data[i]*this->data[i];
        for(size_t i = 0; i < N; i++) this->data[i]/=length;
        return *this;
    }
};

using vector2i = vector<int, 2>;
using vector3i = vector<int, 3>;
using vector4i = vector<int, 4>;
using vector2d = vector<double, 2>;
using vector3d = vector<double, 3>;
using vector4d = vector<double, 4>;
using vector2f = vector<float, 2>;
using vector3f = vector<float, 3>;
using vector4f = vector<float, 4>;

template <typename T> using vector2 = vector<T, 2>;
template <typename T> using vector3 = vector<T, 3>;
template <typename T> using vector4 = vector<T, 4>;

template <typename T, std::size_t N, std::size_t M>
struct matrix : public DataContainer<T, N*M> {
    static_assert(N!=0 || M!=0, "Matrix size can't be zero.");

    static constexpr std::size_t size = N*M;
    static constexpr std::size_t n = N;
    static constexpr std::size_t m = M;

    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const DataContainer<T, N*M>& container) {
        std::copy(container.data, container.data + size, this->data);
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