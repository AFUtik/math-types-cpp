#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "container.hpp"

#define INIT_XYZW_RGBA \
    x(this->data[0]), \
    y(N>1 ? this->data[1] : this->data[0]), \
    z(N>2 ? this->data[2] : this->data[0]), \
    w(N>3 ? this->data[3] : this->data[0]), \
    r(this->data[0]), \
    g(N>1 ? this->data[1] : this->data[0]), \
    b(N>2 ? this->data[2] : this->data[0]), \
    a(N>3 ? this->data[3] : this->data[0]) \

namespace mtp {

template <typename T, std::size_t N, std::size_t Precition = 6>
struct vector : public DataContainer<T, N, Precition> {
    static_assert(N!=0, "Vector size can't be zero.");

    T &x, &y, &z, &w;
    T &r, &g, &b, &a;

    constexpr vector() : DataContainer<T, N, Precition>(), INIT_XYZW_RGBA {}

    constexpr vector(T scalar) : DataContainer<T, N, Precition>(scalar), INIT_XYZW_RGBA {}

    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
    constexpr vector(Args... args) : DataContainer<T, N, Precition>(args...), INIT_XYZW_RGBA {}

    template <std::size_t NN, typename... Args, typename = std::enable_if_t<sizeof...(Args) + NN == N>>
    constexpr vector(const vector<T, NN> &vec, const Args&... args) : INIT_XYZW_RGBA {
        if constexpr (std::is_class_v<T>) std::copy(vec.data, vec.data+NN, this->data);
        else memcpy(this->data, vec.data, vec.MEMORY_SIZE);

        std::size_t i = NN;
        ((this->data[i++] = args), ...);
    }

    /* Data assigning */

    constexpr vector(const DataContainer<T, N>& container) : INIT_XYZW_RGBA 
    {
        if constexpr (std::is_class_v<T>) std::copy(container.data, container.data+N, this->data);
        else memcpy(this->data, container.data, this->MEMORY_SIZE);
    }

    constexpr inline void operator=(const vector<T, N>& container) {
        x = container.x; r = container.r;
        y = container.y; g = container.g;
        z = container.z; b = container.b;
        w = container.w; a = container.a;
    }

    /* UTILS methods */
    constexpr inline vector& normalize() {
        double length = 0.0f;
        for(size_t i = 0; i < N; i++) length += this->data[i]*this->data[i];
        length = mtp::sqrt<double>(length);
        for(size_t i = 0; i < N; i++) this->data[i]/=length;
        return *this;
    }
};

/* static methods for vector */
struct vector_st {
    template<typename CastType, typename T, std::size_t N>
    static constexpr inline vector<CastType, N> cast(const vector<T, N> &container) {
        vector<CastType, N> new_container;
        for(std::size_t i = 0; i < N; i++) new_container.data[i] = static_cast<CastType>(container.data[i]);
        return new_container;
    }

    template<typename T, std::size_t N>
    static constexpr inline vector<T, N> normalize(const vector<T, N> &vec) {
        vector<T, N> new_container;
        double length = 0.0f;
        for(size_t i = 0; i < N; i++) length += vec.data[i]*vec.data[i];
        length = mtp::sqrt<double>(length);
        for(size_t i = 0; i < N; i++) new_container.data[i]/=length;
        return new_container;
    }
};

/* the vector that stores pointers and frees them */
template <typename T, std::size_t N>
struct vector_ptr : public DynamicDataContainerWrapper<T, N> {
    static_assert(N!=0, "Vector size can't be zero.");
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

}

#endif