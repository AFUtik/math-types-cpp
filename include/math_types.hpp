#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <type_traits>
#include <algorithm>

/* Namespace Math Type*/
namespace mtp {

static constexpr std::size_t power10(std::size_t p, std::size_t result = 1) {
    return (p == 0) ? result : power10(p - 1, result * 10);
}

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DataContainer {
    static constexpr float epsilon = 1.0f / static_cast<float>(power10(Precition));
    T data[Size];
    
    constexpr DataContainer() : data{} 
    {
        
    }

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == Size>>
    constexpr DataContainer(Args... args) : data{static_cast<T>(args)...}
    {

    }

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

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() + std::declval<U>()), U>>>
    constexpr inline DataContainer operator+(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() - std::declval<U>()), U>>>
    constexpr inline DataContainer operator-(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    
    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() * std::declval<U>()), U>>>
    constexpr inline DataContainer operator*(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * other.data[i];
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() / std::declval<U>()), U>>>
    constexpr inline DataContainer operator/(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / other.data[i];
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() + std::declval<U>()), U>>>
    constexpr inline DataContainer operator+(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() - std::declval<U>()), U>>>
    constexpr inline DataContainer operator-(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() * std::declval<U>()), U>>>
    constexpr inline DataContainer operator*(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() / std::declval<U>()), U>>>
    constexpr inline DataContainer operator/(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() += std::declval<U>()), U>>>
    constexpr inline DataContainer& operator+=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] += other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() -= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator-=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] -= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() *= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator*=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] *= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() /= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator/=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] /= other.data[i];
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() += std::declval<U>()), U>>>
    constexpr inline DataContainer& operator+=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] += scalar;
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() -= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator-=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] -= scalar;
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() *= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator*=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] *= scalar;
        return *this;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() /= std::declval<U>()), U>>>
    constexpr inline DataContainer& operator/=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] /= scalar;
        return *this;
    }

    /* Comparison operators. Returns only bitmask */

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() > std::declval<U>()), U>>>
    constexpr inline int operator>(const DataContainer& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] > other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() < std::declval<U>()), U>>>
    constexpr inline int operator<(const DataContainer& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] < other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() == std::declval<U>()), U>>>
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

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<U>() != std::declval<U>()), U>>>
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

template <typename T, std::size_t N, std::size_t Flag = 0>
struct vector : public DataContainer<T, N> {
    static constexpr std::size_t size = N;
    static constexpr std::size_t flag = Flag;

    using DataContainer<T, N>::DataContainer;

    constexpr vector(const DataContainer<T, N>& container) {
        std::copy(container.data, container.data + size, this->data);
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
using vector2ul = vector<unsigned long long, 2>;
using vector3ul = vector<unsigned long long, 3>;
using vector4ul = vector<unsigned long long, 4>;
using vector2l = vector<long long, 2>;
using vector3l = vector<long long, 3>;
using vector4l = vector<long long, 4>;
using vector2ui = vector<unsigned int, 2>;
using vector3ui = vector<unsigned int, 3>;
using vector4ui = vector<unsigned int, 4>;
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

/* DATACONTAINER USING WITH FLAGS */

template <std::size_t Flag> using vector2df = vector<double, 2, Flag>;
template <std::size_t Flag> using vector3df = vector<double, 3, Flag>;
template <std::size_t Flag> using vector4df = vector<double, 4, Flag>;
template <std::size_t Flag> using vector2ff = vector<float, 2, Flag>;
template <std::size_t Flag> using vector3ff = vector<float, 3, Flag>;
template <std::size_t Flag> using vector4ff = vector<float, 4, Flag>;

template <typename T, std::size_t N, std::size_t M, std::size_t Flag = 0>
struct matrix : public DataContainer<T, N*M> {
    static constexpr std::size_t size = N*M;
    static constexpr std::size_t n = N;
    static constexpr std::size_t m = M;
    static constexpr std::size_t flag = Flag;

    using DataContainer<T, N*M>::DataContainer;

    constexpr matrix(const DataContainer<T, N*M>& container) {
        std::copy(container.data, container.data + size, this->data);
    }

    /* Utils */

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

/* matrix using with flags */

template <std::size_t Flag> using matrix4df = matrix<double, 4, 4, Flag>;
template <std::size_t Flag> using matrix3df = matrix<double, 3, 3, Flag>;
template <std::size_t Flag> using matrix2df = matrix<double, 2, 2, Flag>;
template <std::size_t Flag> using matrix4ff = matrix<float, 4, 4, Flag>;
template <std::size_t Flag> using matrix3ff = matrix<float, 3, 3, Flag>;
template <std::size_t Flag> using matrix2ff = matrix<float, 2, 2, Flag>;

}

#endif