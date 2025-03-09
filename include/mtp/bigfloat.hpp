#ifndef BIGFLOAT_HPP
#define BIGFLOAT_HPP

#include <string>

/**
* @brief Can storage numbers with floating point from
* -1.761×10^646456993 to 1.761×10^646456993.
*/
template <std::size_t Mantissa = 1>
class bigfloat {
private:
    uint32_t exponent;
    uint64_t mantissa[Mantissa];
public:

using bigf64_t = bigfloat<1>;
using bigf128_t = bigfloat<2>;
using bigf256_t = bigfloat<4>;
using bigf512_t = bigfloat<8>;

};
#endif