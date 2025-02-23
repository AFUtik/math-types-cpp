#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "vector.hpp"
#include "matrix.hpp"

namespace mtp {

template<typename T>
struct transform2 {
    vector2<T> position;
    matrix2<T> rot_mat = {1, 0,
                          0, 1};
    float angle = 0.0f; /* in radians */
    vector2<T> scale = {1, 1};
};

template<typename T>
struct transform3 {
    vector3<T> position;
    matrix3<T> rot_mat = {1, 0, 0,
                          0, 1, 0,
                          0, 0, 1};
    vector4<T> quat = {0, 0, 0, 1};
    vector3<T> scale= {1, 1, 1};
};

struct st_transform {
    template <typename T>
    static constexpr inline void translate(matrix<T, 4, 4> &model, const vector<T, 3> &vector) {
        model.data[12] += vector.data[0];
        model.data[13] += vector.data[1];
        model.data[14] += vector.data[2];
    };

    template <typename T>
    static constexpr inline void translate(matrix<T, 4, 4> &model, const vector<T, 2> &vector) {
        model.data[12] += vector.data[0];
        model.data[13] += vector.data[1];
    };

    template <typename T>
    static constexpr inline void translate(matrix<T, 3, 3> &model, const vector<T, 2> &vector) {
        model.data[6] += vector.data[0];
        model.data[7] += vector.data[1];
    };

    template <typename T>
    static constexpr inline void setPos(matrix<T, 4, 4> &model, const vector<T, 3> &vector) {
        model.data[12] = vector.data[0];
        model.data[13] = vector.data[1];
        model.data[14] = vector.data[2];
    };

    template <typename T>
    static constexpr inline void setPos(matrix<T, 4, 4> &model, const vector<T, 2> &vector) {
        model.data[12] = vector.data[0];
        model.data[13] = vector.data[1];
    };
    
    template <typename T>
    static constexpr inline void setPos(matrix<T, 3, 3> &model, const vector<T, 2> &vector) {
        model.data[6] = vector.data[0];
        model.data[7] = vector.data[1];
    };

    template <typename T>
    static constexpr inline void setRot(matrix<T, 4, 4> &model, const matrix<T, 3, 3> &vector) {
        model.data[0] = vector.data[0];
        model.data[1] = vector.data[1];
        model.data[2] = vector.data[2];
        model.data[4] = vector.data[3];
        model.data[5] = vector.data[4];
        model.data[6] = vector.data[5];
        model.data[8] = vector.data[6];
        model.data[9] = vector.data[7];
        model.data[10]= vector.data[8];
    };

    template <typename T>
    static constexpr inline void setRot(matrix<T, 3, 3> &model, const matrix<T, 2, 2> &vector) {
        model.data[0] = vector.data[0];
        model.data[1] = vector.data[1];
        model.data[3] = vector.data[2];
        model.data[4] = vector.data[3];
    };
    
};

using transform2d = transform2<double>;
using transform3d = transform3<double>;
using transform2f = transform2<float>;
using transform3f = transform3<float>;

}

#endif