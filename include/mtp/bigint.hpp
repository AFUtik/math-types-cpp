#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <string>
#include <iostream>

#define BLOCK_SIZE 10ULL
#define BASE 10'000'000'000ULL

#define ABS(i1) (i1 < 0 ? -i1 : i1)
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)
#define MAX(i1, i2) (i1 > i2 ? i1 : i2)

namespace mtp {

template <size_t Integers>
class bigint {
public:  
    bool is_signed = false;
    size_t last_int = 1;
    uint64_t nums[Integers] {};

    bigint() {}

    bigint(const uint64_t &value) {
        nums[0] = value;
    }

    bigint(const std::string &string) {
        size_t str_size = string.size();
        if (!string.empty() && string[0] == '-') is_signed = true;

        uint32_t carry = 0;
        for(size_t i = is_signed; i < str_size; i+=BLOCK_SIZE) {
            const size_t &len = MIN(BLOCK_SIZE, str_size - i);
            std::cout << len << std::endl;

            uint64_t value = 0;
            for(size_t j = 0; j < len; j++) {
                value = value * 10 + (string[i+j]-'0');
            }

            carry = value;
            for(size_t j = 0; j < Integers; j++) { 
                const uint64_t sum = (uint64_t)nums[j] + carry;
                nums[j] = sum & 0xFFFFFFFF;
                carry = sum >> 32;
                
                if(carry == 0) break;
            }
        }
    }
    
    const std::string str() {
        return {};
    }

    bigint operator+(const bigint &other) {
        bigint<Integers> result;

        const size_t& min_size = MIN(last_int, other.last_int);
        bool carry = 0;
        for(size_t i = 0; i < Integers; ++i) { 
            const uint32_t &b = other.nums[i];
            const uint32_t sum = nums[i] + b + carry;
            nums[i] = sum;
            carry = sum < b;

            if(carry == 0 && sum == 0 && min_size < i) break;
        }
    }

    bigint operator-(const bigint &other) {
        bigint<Integers> result;

        bool carry = 0;
        for(size_t i = 0; i < Integers; i++) { 
            const uint32_t sub = nums[i] - other.nums[i] - carry;
            nums[i] = sub;
            carry = nums[i] < other.nums[i];
            
            if(carry == 0 && sub==0) break;
        }
        return result;
    }

    bigint operator*(const bigint &other) {
        bigint result;
        if(other.is_signed && is_signed) result.is_signed = false;

        uint64_t carry = 0;
        for (size_t i = 0; i < last_int; i++) {
            uint64_t product = (uint64_t)nums[i] * other.nums[i] + carry;
            result.nums[i] = product & 0xFFFFFFFF;
            carry = product >> 32;
        }
    }

    bigint operator/(const bigint &other) {
        if(other.is_signed && is_signed) is_signed = false;
    }

    void operator+=(const bigint & __restrict other) noexcept {
        bool carry = 0;
        for (size_t i = 0; i < Integers; ++i) { 
            uint64_t &a = nums[i];
            const uint64_t &b = other.nums[i];
            const uint64_t sum = a + b + carry;
            carry = sum < a;
            a = sum;
        }
    }

    void operator-=(const bigint & __restrict other) noexcept {
        bool carry = 0;
        for (size_t i = 0; i < Integers; ++i) { 
            uint64_t &a = nums[i];
            const uint64_t &b = other.nums[i];
            const uint64_t sub = a - b - carry;
            carry = a < b;
            a = sub;
        }
    }

    void operator*=(const bigint & __restrict other) noexcept {
        if(other.is_signed && is_signed) is_signed = false;
        bigint result;
        __uint128_t carry = 0;
        for (size_t i = 0; i < Integers; i++) {
            carry = 0;
            for (size_t j = 0; j < Integers; j++) {
                __uint128_t product = (__uint128_t)nums[j] * other.nums[i] + carry;
                result.nums[j]     += (uint64_t)product;
                carry = product >> 64;
            }
        }
        memcpy(nums, result.nums, sizeof(uint64_t)*Integers);
    }

    void operator/=(const bigint &other) noexcept {

    }

    bigint operator+(const int &other) {

    }

    bigint operator-(const int &other) {

    }

    bigint operator*(const int &other) {
        bigint result;

    }

    bigint operator/(const int &other) {
        bigint result;
    }

    void operator+=(const unsigned int &other) noexcept {
        uint64_t carry = 0;
        for(size_t i = 0; i < Integers; i++) { 
            const uint64_t sum = (uint64_t)nums[i] + other + carry;
            nums[i] = sum & 0xFFFFFFFF;
            carry = sum >> 32;
            
            if(carry == 0 && sum==0) break;
        }
    }

    void operator-=(const int &other) noexcept {

    }

    void operator*=(const int &other) noexcept {

    }

    void operator/=(const int &other) noexcept {

    }

    void operator++() noexcept {this+=1;}
    void operator--() noexcept {this-=1;}
};

using bigint128_t = bigint<2>;
using bigint256_t = bigint<4>;
using bigint512_t = bigint<8>;

}

#endif