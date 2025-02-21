#ifndef SSE_CALC_HPP
#define SSE_CALC_HPP

#ifndef __SSE4_1__
    #if defined(__clang__)
        #pragma message "SSE4.1 is disabled. Use -msse4.1 or -march=native parameter to enable this feature."
        #define SSE4_1
    #elif defined(__GNUC__) 
        #pragma GCC target("sse4.1")
        #define SSE4_1 1
    #elif defined(_MSC_VER)
        #pragma message("SSE4.1 is disabled. Use flag /arch:SSE4.1 to enable this feature.")
        #define SSE4_1
    #endif
#endif

#include <type_traits>
#include <emmintrin.h>
#include <smmintrin.h>

typedef __m128d D2; /* float_64 2 */
typedef __m128  F4; /* float_32 4 */

typedef __m128i L2; /* int_64 2 */
typedef __m128i I4; /* int_32 4 */
typedef __m128i S8; /* int_16 8 */
typedef __m128i B16;/* int_8 16 */

template <typename T>
struct m128 { using type = void; };

template <> struct m128<float>              { using type = F4; };
template <> struct m128<double>             { using type = D2; };
template <> struct m128<long>               { using type = I4; };
template <> struct m128<int>                { using type = I4; };
template <> struct m128<short>              { using type = I4; };
template <> struct m128<char>               { using type = I4; };
template <> struct m128<unsigned long long> { using type = I4; };
template <> struct m128<unsigned long>      { using type = I4; };
template <> struct m128<unsigned int>       { using type = I4; };
template <> struct m128<unsigned short>     { using type = I4; };
template <> struct m128<unsigned char>      { using type = I4; };

template <typename T>
using m128_t = typename m128<T>::type;

namespace sse {

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> load(const T* data) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_load_ps(data);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_load_pd(data);
    } else if constexpr (std::is_integral_v<T>) {
        return _mm_load_si128(reinterpret_cast<const __m128i*>(data));
    } else {
        return {};
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline void store(T* data, const m128_t<T> &m_data) {
    if constexpr (std::is_same_v<T, float>) {
        _mm_store_ps(data, m_data);
    } else if constexpr (std::is_same_v<T, double>) {
        _mm_store_pd(data, m_data);
    } else if constexpr (std::is_integral_v<T>) {
        _mm_store_si128(reinterpret_cast<__m128i*>(data), m_data);
    }
}

/* numeric computations */

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> pow2(const m128_t<T> &m_data) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_mul_ps(m_data, m_data);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_mul_pd(m_data, m_data);
    } else if constexpr (std::is_same_v<T, long> || std::is_same_v<T, unsigned long>) {
        return _mm_mullo_epi64(m_data, m_data);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_mullo_epi32(m_data, m_data);
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_mullo_epi16(m_data, m_data);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> sqrt(const m128_t<T> &m_data) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_sqrt_ps(m_data, m_data);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_sqrt_pd(m_data, m_data);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> mul(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_mul_ps(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_mul_pd(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_mullo_epi64(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_mullo_epi32(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_mullo_epi16(m_data1, m_data2);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> div(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_div_ps(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_div_pd(m_data1, m_data2);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> sum(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_add_ps(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_add_pd(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_add_epi64(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_add_epi32(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_add_epi16(m_data1, m_data2);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> sub(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_sub_ps(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_sub_pd(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_sub_epi64(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_sub_epi32(m_data1, m_data2);
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_sub_epi16(m_data1, m_data2);
    } else {
        return NULL;
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline m128_t<T> dot_product(const m128_t<T> &m_data1, const m128_t<T> &m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        _mm_dp_ps(m_data1, m_data2, 0xF1);
    } else if constexpr (std::is_same_v<T, double>) {
        _mm_dp_pd(m_data1, m_data2, 0xF1);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        I4 mul_res = _mm_mullo_epi32(m_data1, m_data2);
        I4 hadd1 = _mm_hadd_epi32(mul_res, mul_res);
        return _mm_hadd_epi32(hadd1, hadd1); 
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        I4 mul_res = _mm_mullo_epi16(m_data1, m_data2);
        I4 hadd1 = _mm_hadd_epi16(mul_res, mul_res);
        return _mm_hadd_epi16(hadd1, hadd1); 
    } else {
        return NULL;
    }
}

/* comparisons */

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline bool equal(const m128_t<T>& m_data1, const m128_t<T>& m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_movemask_ps(_mm_cmpeq_ps(m_data1, m_data2)) == 0b1111;
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_movemask_pd(_mm_cmpeq_pd(m_data1, m_data2)) == 0b11;
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi64(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi32(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi16(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi8(m_data1, m_data2)) == 0xFFFF;
    } else {
       return false; 
    }
}

/* Returns true if the first parameter greater than the second parameter */
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline bool greater(const m128_t<T>& m_data1, const m128_t<T>& m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_movemask_ps(_mm_cmpgt_ps(m_data1, m_data2)) == 0b1111;
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_movemask_pd(_mm_cmpgt_pd(m_data1, m_data2)) == 0b11;
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_movemask_epi8(_mm_cmpgt_epi64(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_movemask_epi8(_mm_cmpgt_epi32(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_movemask_epi8(_mm_cmpgt_epi16(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
        return _mm_movemask_epi8(_mm_cmpgt_epi8(m_data1, m_data2))  == 0xFFFF;
    } else {
        return false;
    }
}

/* Returns true if the first parameter equals or greater than the second parameter */
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline bool greater_or_equal(const m128_t<T>& m_data1, const m128_t<T>& m_data2) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_movemask_ps(_mm_cmpge_ps(m_data1, m_data2)) == 0b1111;
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_movemask_pd(_mm_cmpge_pd(m_data1, m_data2)) == 0b11;
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi64(m_data1, m_data2)) == 0xFFFF ||
               _mm_movemask_epi8(_mm_cmpgt_epi64(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi32(m_data1, m_data2)) == 0xFFFF ||
               _mm_movemask_epi8(_mm_cmpgt_epi32(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, short> || std::is_same_v<T, unsigned short>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi16(m_data1, m_data2)) == 0xFFFF ||
               _mm_movemask_epi8(_mm_cmpgt_epi16(m_data1, m_data2)) == 0xFFFF;
    } else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
        return _mm_movemask_epi8(_mm_cmpeq_epi8(m_data1, m_data2))  == 0xFFFF ||
               _mm_movemask_epi8(_mm_cmpgt_epi8(m_data1, m_data2))  == 0xFFFF;
    } else {
        return false;
    }
}

/* conversions */

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline I4 to_int(const m128_t<T> &m_data1) {
    if constexpr (std::is_same_v<T, float>) {
        return _mm_cvttps_epi32(m_data1);
    } else if constexpr (std::is_same_v<T, double>) {
        return _mm_cvttps_epi64(m_data1);
    } else {
        return _mm_setzero_si128();
    }
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline F4 to_float(const m128_t<T> &m_data1) {
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return _mm_cvtepi32_ps(m_data1); 
    } else {
        return _mm_setzero_ps();
    }
}

}

#endif