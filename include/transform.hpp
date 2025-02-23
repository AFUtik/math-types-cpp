#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "math_types.hpp"

namespace mtp {

struct transform {
    template <
        typename T, std::size_t MN, std::size_t MM,
        typename V, std::size_t N
    >
    static constexpr inline void translate(mtp::matrix<T, MN, MM> &model, const mtp::vector<V, N> &vector) {
        static_assert(MM >= 1 || MN >= 1, "Size must be greater than one.");
        constexpr std::size_t POS_INDEX = (MN*MM)-MN;
        for(uint32_t i = POS_INDEX, j = 0; j < N; i++, j++) model.data[i] += vector.data[j];
    };

    template <
        typename T, std::size_t MN, std::size_t MM,
        typename V, std::size_t N
    >
    static constexpr inline void setPosition(mtp::matrix<T, MN, MM> &model, const mtp::vector<V, N> &vector) {
        static_assert(MM >= 1 || MN >= 1, "Size must be greater than one.");
        constexpr std::size_t POS_INDEX = (MN*MM)-MN;
        for(uint32_t i = POS_INDEX, j = 0; j < N; i++, j++) model.data[i] = vector.data[j];
    };

    template <
        typename T, std::size_t MN, std::size_t MM,
        typename V, std::size_t N,  std::size_t M
    >
    static constexpr inline void setRotation(mtp::matrix<T, MN, MM> &model, const mtp::matrix<V, N, M> &matrix) {
        static_assert(N == M, "Rotation matrix must be square.");
        static_assert(MN >= N && MM >= M, "Target matrix must be large enough.");
        
        for (std::size_t row = 0; row < N; row++) {
            for (std::size_t col = 0; col < M; col++) {
                model.data[row * MN + col] = matrix.data[row * M + col];
            }
        }
    };
};

}

#endif