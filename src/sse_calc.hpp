#ifndef SSE_CALC_HPP
#define SSE_CALC_HPP

#ifdef _MSC_VER
    #include <intrin.h>
#elif __GNUC__
    #pragma GCC target("sse4.1")

    #include <smmintrin.h>
#endif

#include <type_traits>
#include <emmintrin.h>

typedef __m128  F4; /* float 4*/
typedef __m128d D2; /* double 2*/
typedef __m128i I4; /* int 4*/

template <typename T>
struct m128 { using type = void; };

template <> struct m128<float>          { using type = F4; };
template <> struct m128<double>         { using type = D2; };
template <> struct m128<int>            { using type = I4; };
template <> struct m128<short>          { using type = I4; };
template <> struct m128<char>           { using type = I4; };
template <> struct m128<unsigned int>   { using type = I4; };
template <> struct m128<unsigned short> { using type = I4; };
template <> struct m128<unsigned char>  { using type = I4; };

template <typename T>
using m128_t = typename m128<T>::type;

namespace sse {

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> load(const T* data) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_loadu_ps(data);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_loadu_pd(data);
    } else {
        return _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void store(const T* data, const m128_t<T> &m_data) {
    if constexpr (std::is_same_v<T, float>) {
        _mm_storeu_ps(data, m_data);
    } else if constexpr (std::is_same_v<T, double>) {
        _mm_storeu_pd(data, m_data);
    } else {
        _mm_storeu_si128(reinterpret_cast<const __m128i*>(data), m_data);
    }
}

/* numeric computations */

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void pow2(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void sqrt(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void mul(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void div(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void sum(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void sub(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void dot_product(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {

    } else if constexpr (std::is_same_v<T, double>) {

    } else {

    }
}

/* comparisons */

#define MASK_EQUAL_F4(DATA1, DATA2) 
#define EQUAL_F4(DATA1, DATA2)      /* DATA1 == DATA2 */
#define NOT_EQUAL_F4(DATA1, DATA2)  /* DATA1 != DATA2 */

#define MASK_EQUAL_D4(DATA1, DATA2)
#define EQUAL_D4(DATA1, DATA2)
#define NOT_EQUAL_D4(DATA1, DATA2)

#define MASK_GREATER_F4(DATA1, DATA2) 
#define GREATER_F4(DATA1, DATA2) /* DATA1 > DATA2 */
#define LESS_F4(DATA1, DATA2)    /* DATA1 < DATA2 */

#define MASK_GREATER_D4(DATA1, DATA2)
#define GREATER_D4(DATA1, DATA2)
#define LESS_D4(DATA1, DATA2)

#define MASK_GREATER_OR_EQUAL_F4(DATA1, DATA2)
#define GREATER_OR_EQUAL_F4(DATA1, DATA2) /* DATA1 >= DATA2 */
#define LESS_OR_EQUAL_F4(DATA1, DATA2)    /* DATA1 <= DATA2 */

#define MASK_GREATER_OR_EQUAL_D4(DATA1, DATA2)
#define GREATER_OR_EQUAL_D4(DATA1, DATA2)
#define LESS_OR_EQUAL_D4(DATA1, DATA2)

/* conversions */

#define FLOAT_TO_INT(DATA1, DATA2)
#define INT_TO_FLOAT(DATA1, DATA2)

#endif

