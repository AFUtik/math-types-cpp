/**
* @file container.hpp
* @brief MathType File.
* @author AFUtik
* @date 2025-07-04
* @version v1.0.0-dev
* @copyright MIT
*
* @details Static container realization and dynamic container realization.
*/

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <type_traits>
#include <algorithm>

#include "utils/constfunc.hpp"

namespace mtp {

template <typename T, std::size_t Size, std::size_t Precition = 6>
struct DataContainer {
    union {
        T data[Size];
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };

    static constexpr float EPSILON = 1.0f / static_cast<float>(mtpu::pow10(Precition));

    constexpr DataContainer() : data{} 
    {

    }

    constexpr DataContainer(T (&arr)[Size])
    {  
        std::copy(arr, arr+Size, data);
    }

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == Size>>
    constexpr DataContainer(const Args&... args) : data{static_cast<T>(args)...} 
    {

    }

    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    constexpr DataContainer(const T& scalar)
    {
        for(std::size_t i = 0; i < Size; i++) data[i] = scalar;
    }

    using data_iterator  = T*;
    using data_citerator = const T*;

    constexpr data_iterator begin() noexcept { return data; }
    constexpr data_iterator end() noexcept { return data + Size; }

    constexpr data_citerator cbegin() const noexcept { return data; }
    constexpr data_citerator cend() const noexcept { return data + Size; }

    constexpr inline T& operator[](const std::size_t &index) {return data[index];}

    constexpr inline const T& operator[](const std::size_t &index) const {return data[index];}

    constexpr inline DataContainer operator+(const T &scalar) const noexcept {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    constexpr inline DataContainer operator-(const T &scalar) const noexcept {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    constexpr inline DataContainer operator*(const T &scalar) const noexcept {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    constexpr inline DataContainer operator/(const T &scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    constexpr inline void operator+=(const T &scalar) noexcept {for (size_t i = 0; i < Size; i++) data[i] += scalar;}

    constexpr inline void operator-=(const T &scalar) noexcept {for (size_t i = 0; i < Size; i++) data[i] -= scalar;}

    constexpr inline void operator*=(const T &scalar) noexcept {for (size_t i = 0; i < Size; i++) data[i] *= scalar;}

    constexpr inline void operator/=(const T &scalar) noexcept {for (size_t i = 0; i < Size; i++) data[i] /= scalar;}

    constexpr inline void operator<<=(const T &scalar) noexcept {for (size_t i = 0; i < Size; i++) data[i] <<= scalar;}

    /* Comparison operators. Returns only bitmask */

    constexpr int operator>=(const DataContainer<T, Size>& other) const {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] >= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator<=(const DataContainer<T, Size>& other) const {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] <= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator>(const DataContainer<T, Size>& other) const {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] > other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator<(const DataContainer<T, Size>& other) const {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] < other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator==(const DataContainer<T, Size>& other) const {
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
    constexpr inline int operator!=(const DataContainer<T, Size>& other) const {
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

    constexpr inline size_t size() const noexcept {return Size;}
};

template <typename T>
struct DataContainer<T, 0> {
    T* data;

    DataContainer() : data(nullptr), __size(0)
    {

    }

    DataContainer(const std::size_t& size) : data(new T[size]()), __size(size)
    {

    }

    DataContainer(const std::size_t& size, const T& scalar) : data(new T[size]), __size(size)
    {
        for(size_t i = 0; i < size; i++) data[i] = scalar;
    }

    ~DataContainer() 
    {
        delete[] data;
    }

    constexpr inline T& operator[](const std::size_t &index) {return data[index];}

    constexpr inline const T& operator[](const std::size_t &index) const {return data[index];}

    inline void operator+=(const T &scalar) {for (size_t i = 0; i < __size; i++) data[i] += scalar;}

    inline void operator-=(const T &scalar) {for (size_t i = 0; i < __size; i++) data[i] -= scalar;}

    inline void operator*=(const T &scalar) {for (size_t i = 0; i < __size; i++) data[i] *= scalar;}

    inline void operator/=(const T &scalar) {for (size_t i = 0; i < __size; i++) data[i] /= scalar;}

    inline void operator<<=(const T &scalar){for (size_t i = 0; i < __size; i++) data[i] <<= scalar;}

    inline DataContainer operator+(const T &scalar) const {
        DataContainer result(__size);
        for (size_t i = 0; i < __size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    inline DataContainer operator-(const T &scalar) const {
        DataContainer result(__size);
        for (size_t i = 0; i < __size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    inline DataContainer operator*(const T &scalar) const {
        DataContainer result(__size);
        for (size_t i = 0; i < __size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    inline DataContainer operator/(const T &scalar) const {
        DataContainer result(__size);
        for (size_t i = 0; i < __size; i++) result.data[i] = data[i] / scalar;
        return result;
    }

    inline void resize(const size_t &size) {
        data = new T[size]{};
    }

    inline const size_t& size() const noexcept {return __size;}
protected:
    size_t __size = 0;
};

/**
* @brief finds max value of a container 
*/
template <typename T, std::size_t Size>
static constexpr inline T max(const DataContainer<T, Size> &container) {
    T max = 0;
    for(size_t i = 0; i < container.size(); i++) if(max < container[i]) max = container[i];
    return max;
}

/**
* @brief finds min value of a container 
*/
template <typename T, std::size_t Size>
constexpr inline T min(const DataContainer<T, Size> &container) {
    T min = container.data[0];
    for(size_t i = 1; i < container.size(); i++) if(min > container[i]) min = container[i];
    return min;
}

/* works with predefined sizes */
template <std::size_t NewSize, typename T, std::size_t OldSize>
constexpr inline DataContainer<T, NewSize> static_resize(const DataContainer<T, OldSize> &container) noexcept {
    DataContainer<T, NewSize> new_container;
    
    for(std::size_t i = 0; i < std::min(OldSize, NewSize); i++) new_container.data[i] = container.data[i];
    return new_container;
}

template <typename T, typename CastType, std::size_t Size>
constexpr inline DataContainer<CastType, Size> cast(const DataContainer<T, Size> &container) noexcept {
    DataContainer<CastType, Size> new_container;
    for(std::size_t i = 0; i < Size; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
    return new_container;
}

}

#endif