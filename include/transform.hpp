#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "math_types.hpp"

namespace mtp {

enum vectorTransformFlags {
    VEC_NULL,
    VEC_POS,
    VEC_SCALE,
    QUAT
};

enum matrixTransformFlags {
    MATRIX_NULL,
    MAT_MODEL,
    MAT_ROT
};

struct transform {
    template <
        typename T, std::size_t MN, std::size_t MM,   std::size_t MFlag,
        typename V, std::size_t N,  std::size_t VFlag
    >
    static constexpr inline void translate(mtp::matrix<T, MN, MM, MFlag> &model, const mtp::vector<V, N, VFlag> &vector) {
        static_assert(MM >= 1 || MN >= 1, "Size must be greater than one.");
        constexpr std::size_t POS_INDEX = (MN*MM)-MN;
        for(uint32_t i = POS_INDEX, j = 0; j < N; i++, j++) model.data[i] = vector.data[j];
    };

    template <
        typename T, std::size_t MN, std::size_t MM, std::size_t M1Flag,
        typename V, std::size_t N,  std::size_t M,  std::size_t M2Flag
    >
    static constexpr inline void rotate(mtp::matrix<T, MN, MM, M1Flag> &model, const mtp::matrix<V, N, M, M2Flag> &matrix) {
        static_assert(matrix.m == matrix.n, "N and M must be the same size.");
        for(uint32_t i = 0, size = 0; i < matrix.size; size+=matrix.m-1) {
            for(uint32_t j = 0; j < matrix.n; j++) {
                model[size]=matrix[i];
                i++;
            }
        }
    };

    template <
        typename T, std::size_t MN, std::size_t MM,   std::size_t MFlag,
        typename V, std::size_t N,  std::size_t VFlag
    >
    static constexpr inline void rotate(mtp::matrix<T, MN, MM, MFlag> &model, const mtp::vector<V, N, VFlag> &vector) {
        static_assert(MM >= 1 || MN >= 1, "Size must be greater than one.");
        constexpr std::size_t POS_INDEX = (MN*MM)-MN;
        for(uint32_t i = POS_INDEX, j = 0; j < N; i++, j++) model.data[i] = vector.data[j];
    };

    template <
        typename T, std::size_t MN, std::size_t MM,   std::size_t MFlag,
        typename V, std::size_t N,  std::size_t VFlag
    >
    static constexpr inline void mutate(mtp::matrix<T, MN, MM, MFlag> &model, const mtp::vector<V, N, VFlag> &vector) {
        static_assert(MM >= 1 || MN >= 1, "Size must be greater than one.");
        constexpr std::size_t POS_INDEX = (MN*MM)-MN;
        if constexpr (VFlag==VEC_POS && MN >= N) {
            for(uint32_t i = POS_INDEX, j = 0; j < N; i++, j++)      model.data[i] = vector.data[j];
        } else if constexpr (VFlag==VEC_SCALE) {
            for(uint32_t i = 0, j = 0; i < model.size; i+=MN+1, j++) model.data[i] = vector.data[j];
        }
    };

    template <
        typename T, std::size_t MN, std::size_t MM, std::size_t M1Flag,
        typename V, std::size_t N,  std::size_t M,  std::size_t M2Flag
    >
    static constexpr inline void mutate(mtp::matrix<T, MN, MM, M1Flag> &model, const mtp::matrix<V, N, M, M2Flag> &matrix) {
        if constexpr (M1Flag==MAT_ROT && model.size > matrix.size) {
            static_assert(matrix.m == matrix.n, "N and M must be the same size.");
            for(uint32_t i = 0, size = 0; i < matrix.size; size+=matrix.m-1) {
                for(uint32_t j = 0; j < matrix.n; j++) {
                    model[size]=matrix[i];
                    i++;
                }
            }
        } else if(model.size == matrix.size) model = matrix;
    };
};

}

#endif