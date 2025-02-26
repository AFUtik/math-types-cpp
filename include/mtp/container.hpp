#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <type_traits>
#include <cstring>
#include <algorithm>

#include "constfunc.hpp"

/* Namespace Math Type*/

namespace mtp {

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DataContainer {
    std::conditional_t<std::is_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>, T> data[Size];

    static constexpr std::size_t MEMORY_SIZE = Size*sizeof(T);
    static constexpr std::size_t size = Size;
    static constexpr float EPSILON = 1.0f / static_cast<float>(pow10(Precition));
    
    constexpr DataContainer() : data{} {

    }

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == Size>>
    constexpr DataContainer(Args&&... args) : data{static_cast<T>(args)...} {

    }

    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    constexpr DataContainer(const T& scalar)
    {
        for(std::size_t i = 0; i < Size; i++) data[i] = scalar;
    }

    ~DataContainer() = default;

    using data_iterator  = std::remove_reference_t<T>*;
    using data_citerator = const std::remove_reference_t<T>*;

    constexpr data_iterator begin() noexcept { return data; }
    constexpr data_iterator end() noexcept { return data + Size; }

    constexpr data_citerator cbegin() const noexcept { return data; }
    constexpr data_citerator cend() const noexcept { return data + Size; }

    constexpr inline T& operator[](const std::size_t &index) {return data[index];}

    constexpr inline const T& operator[](const std::size_t &index) const {return data[index];}

    constexpr inline DataContainer operator+(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }

    constexpr inline DataContainer operator-(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }
    
    constexpr inline DataContainer operator*(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * other.data[i];
        return result;
    }

    constexpr inline DataContainer operator/(const DataContainer& other) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / other.data[i];
        return result;
    }

    constexpr inline DataContainer operator+(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    constexpr inline DataContainer operator-(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    constexpr inline DataContainer operator*(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    constexpr inline DataContainer operator/(T scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    constexpr inline void operator+=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] += other.data[i];
    }

    constexpr inline void operator-=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] -= other.data[i];
    }

    constexpr inline void operator*=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] *= other.data[i];
    }

    constexpr inline void operator/=(const DataContainer& other) {
        for (size_t i = 0; i < Size; i++) data[i] /= other.data[i];
    }

    constexpr inline void operator+=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] += scalar;
    }

    constexpr inline void operator-=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] -= scalar;
    }

    constexpr inline void operator*=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] *= scalar;
    }

    constexpr inline void operator/=(T scalar) {
        for (size_t i = 0; i < Size; i++) data[i] /= scalar;
    }

    /* Comparison operators. Returns only bitmask */

    constexpr inline int operator>=(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] >= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr inline int operator<=(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] <= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr inline int operator>(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] > other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr inline int operator<(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] < other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr inline int operator==(const DataContainer<T, Size>& other) {
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
    constexpr inline int operator!=(const DataContainer<T, Size>& other) {
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

template<std::size_t NewSize, typename T, std::size_t OldSize>
static constexpr inline DataContainer<T, NewSize> resize(const DataContainer<T, OldSize> &container) {
    DataContainer<T, NewSize> new_container;
    
    for(std::size_t i = 0; i < std::min(OldSize, NewSize); i++) new_container.data[i] = container.data[i];
    return new_container;
}

template<typename T, typename CastType, std::size_t Size>
static constexpr inline DataContainer<CastType, Size> cast(const DataContainer<T, Size> &container) {
    DataContainer<CastType, Size> new_container;
    for(std::size_t i = 0; i < Size; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
    return new_container;
}


}

#endif