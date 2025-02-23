#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <type_traits>
#include <cstring>
#include <algorithm>

#include "constfunc.hpp"
#include <iostream>
#include <memory>

/* Namespace Math Type*/

namespace mtp {

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DataContainer {
    T data[Size];

    static constexpr std::size_t MEMORY_SIZE = Size*sizeof(T);
    static constexpr std::size_t size = Size;
    static constexpr float EPSILON = 1.0f / static_cast<float>(pow10(Precition));
    
    constexpr DataContainer() : data{} {}

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == Size>>
    constexpr DataContainer(Args... args) : data{static_cast<T>(args)...} {}

    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    constexpr DataContainer(const T& scalar)
    {
        for(std::size_t i = 0; i < Size; i++) data[i] = scalar;
    }

    ~DataContainer() = default;

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
    constexpr inline void operator+=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] += other.data[i];
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() -= std::declval<U>()), U>>
    constexpr inline void operator-=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] -= other.data[i];
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() *= std::declval<U>()), U>>
    constexpr inline void operator*=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] *= other.data[i];
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() /= std::declval<U>()), U>>
    constexpr inline void operator/=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] /= other.data[i];
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() += std::declval<U>()), U>>
    constexpr inline void operator+=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] += scalar;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() -= std::declval<U>()), U>>
    constexpr inline void operator-=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] -= scalar;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() *= std::declval<U>()), U>>
    constexpr inline void operator*=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] *= scalar;
    }

    template<typename U = T, typename = std::void_t<decltype(std::declval<U&>() /= std::declval<U>()), U>>
    constexpr inline void operator/=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] /= scalar;
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
                if(data[i] - other.data[i] < EPSILON) bitmask|=1;
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
                if(data[i] - other.data[i] > EPSILON) bitmask|=1;
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

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DynamicDataContainerWrapper : DataContainer<std::unique_ptr<T>, Size> {
    constexpr inline void allocate() {
        for(std::size_t i = 0; i < Size; i++) this->data[i] = std::make_unique<T>();
    }
};

}

#endif