#ifndef AVX_CALC_HPP
#define AVX_CALC_HPP

#ifndef __AVX__
    #if defined(__clang__)
        #pragma message "AVX is disabled. Use -mavx or -march=native parameter to enable this feature."
        #define AVX
    #elif defined(__GNUC__) 
        #pragma GCC target("avx")
        #define AVX 1
    #elif defined(_MSC_VER)
        #pragma message("AVX is disabled. Use flag /arch:AVX to enable this feature.")
        #define AVX
    #endif
#endif

#ifndef __AVX2__
    #if defined(__clang__)
        #pragma message "AVX2 is disabled. Use -mavx2 or -march=native parameter to enable this feature."
        #define AVX2
    #elif defined(__GNUC__) 
        #pragma GCC target("avx2")
        #define AVX2 1
    #elif defined(_MSC_VER)
        #pragma message("AVX2 is disabled. Use flag /arch:AVX2 to enable this feature.")
        #define AVX2
    #endif
#endif

#include <immintrin.h>



#endif