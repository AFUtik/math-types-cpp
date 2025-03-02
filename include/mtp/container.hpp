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
    static_assert(Size!=0, "DataContainer Size can't be zero.");
    T data[Size];

    static constexpr float EPSILON = 1.0f / static_cast<float>(pow10(Precition));
    static constexpr std::size_t MEMORY_SIZE = Size*sizeof(T);
    static constexpr std::size_t size = Size;
    
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

    constexpr inline DataContainer operator+(const T &scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    constexpr inline DataContainer operator-(const T &scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    constexpr inline DataContainer operator*(const T &scalar) const {
        DataContainer result;
        for (size_t i = 0; i < Size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    constexpr inline DataContainer operator/(const T &scalar) const {
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

    constexpr inline void operator+=(const T &scalar) {for (size_t i = 0; i < Size; i++) data[i] += scalar;}

    constexpr inline void operator-=(const T &scalar) {for (size_t i = 0; i < Size; i++) data[i] -= scalar;}

    constexpr inline void operator*=(const T &scalar) {for (size_t i = 0; i < Size; i++) data[i] *= scalar;}

    constexpr inline void operator/=(const T &scalar) {for (size_t i = 0; i < Size; i++) data[i] /= scalar;}

    /* Comparison operators. Returns only bitmask */

    constexpr int operator>=(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] >= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator<=(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] <= other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator>(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] > other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator<(const DataContainer<T, Size>& other) {
        int bitmask = 0;
        for (size_t i = 0; i < Size; i++) {
            if(data[i] < other.data[i]) bitmask|=1;
            bitmask<<=1;
        }
        return bitmask;
    }

    constexpr int operator==(const DataContainer<T, Size>& other) {
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

template <typename T>
struct DynamicDataContainer {
    T* data;
    std::size_t size;
    
    DynamicDataContainer() : data(nullptr), size(0) 
    {

    }

    DynamicDataContainer(const std::size_t& size) : data(new T[size]()), size(size) 
    {

    }

    
    DynamicDataContainer(const std::size_t& size, T* array) : data(array), size(size)
    {
        
    }
    
    DynamicDataContainer(const std::size_t& size, const T& scalar) : data(new T[size]), size(size) 
    {
        for(std::size_t i = 0; i < size; i++) data[i] = scalar;
    }
    
    ~DynamicDataContainer() {
        if(data) {
            delete[] data;
            data = nullptr;
        }
    }

    using data_iterator  = T*;
    using data_citerator = const T*;

    constexpr data_iterator begin() noexcept { return data; }
    constexpr data_iterator end() noexcept { return data + size; }

    constexpr data_citerator cbegin() const noexcept { return data; }
    constexpr data_citerator cend() const noexcept { return data + size; }

    constexpr inline T& operator[](const std::size_t &index) {return data[index];}

    constexpr inline const T& operator[](const std::size_t &index) const {return data[index];}

    inline void operator+=(const T &scalar) {for (size_t i = 0; i < size; i++) data[i] += scalar;}

    inline void operator-=(const T &scalar) {for (size_t i = 0; i < size; i++) data[i] -= scalar;}

    inline void operator*=(const T &scalar) {for (size_t i = 0; i < size; i++) data[i] *= scalar;}

    inline void operator/=(const T &scalar) {for (size_t i = 0; i < size; i++) data[i] /= scalar;}

    inline DynamicDataContainer operator+(const T &scalar) const {
        DynamicDataContainer result(size);
        for (size_t i = 0; i < size; i++) result.data[i] = data[i] + scalar;
        return result;
    }

    inline DynamicDataContainer operator-(const T &scalar) const {
        DynamicDataContainer result(size);
        for (size_t i = 0; i < size; i++) result.data[i] = data[i] - scalar;
        return result;
    }

    inline DynamicDataContainer operator*(const T &scalar) const {
        DynamicDataContainer result(size);
        for (size_t i = 0; i < size; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    inline DynamicDataContainer operator/(const T &scalar) const {
        DynamicDataContainer result(size);
        for (size_t i = 0; i < size; i++) result.data[i] = data[i] / scalar;
        return result;
    }
};

template <std::size_t NewSize, typename T, std::size_t OldSize>
static constexpr inline DataContainer<T, NewSize> resize(const DataContainer<T, OldSize> &container) {
    DataContainer<T, NewSize> new_container;
    
    for(std::size_t i = 0; i < std::min(OldSize, NewSize); i++) new_container.data[i] = container.data[i];
    return new_container;
}

template <typename T, typename CastType, std::size_t Size>
static constexpr inline DataContainer<CastType, Size> cast(const DataContainer<T, Size> &container) {
    DataContainer<CastType, Size> new_container;
    for(std::size_t i = 0; i < Size; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
    return new_container;
}

}

#endif