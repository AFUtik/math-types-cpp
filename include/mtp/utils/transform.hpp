#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../vector.hpp"
#include "../matrix.hpp"
#include <cmath>

using namespace mtp;

namespace mtpu {

template<typename T>
struct transform {
    matrix4<T> mm = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }; /* Defualt Model Matrix */
};

/* Position tranformations */

template <typename T>
static constexpr inline void translate(matrix<T, 4, 4> &model, const vector<T, 3> &vector) {
    model.data[3] += vector.data[0];
    model.data[7] += vector.data[1];
    model.data[11]+= vector.data[2];
};

template <typename T>
static constexpr inline void translate(matrix<T, 4, 4> &model, const vector<T, 2> &vector) {
    model.data[3] += vector.data[0];
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

/* Rotation transfomations */

/**
* @param model - Model Matrix
* @param vector - Rotations in radians.
* @return Modifies model matrix.
*/
template <typename T>
static constexpr void rotate(matrix<T, 4, 4> &model, const vector<T, 3> &vector) {
    const float ca = std::cos(vector.x), sa = std::sin(vector.x);
    const float cb = std::cos(vector.y), sb = std::sin(vector.y);
    const float cg = std::cos(vector.z), sg = std::sin(vector.z);

    const float mul1 = cg * sa;
    const float mul2 = ca * sb;
    const float mul3 = sg * sa;

    /* Matrix Multiplications - R_x * R_y * R_z */
    model.data[0] = cb * cg;
    model.data[1] = mul1 * sb - sg * ca;
    model.data[2] = cg * mul2 + mul3;

    model.data[4] = sb;
    model.data[5] = cb * ca;
    model.data[6] = -cb * sa;

    model.data[8] = -sg * cb;
    model.data[9] = mul3 * sb + cg * ca;
    model.data[10]= sg * mul2 - mul1;
};

template <typename T>
static constexpr inline void setRotation(matrix<T, 4, 4> &model, const matrix<T, 3, 3> &matrix) {
    model.data[0] = matrix.data[0];
    model.data[1] = matrix.data[1];
    model.data[2] = matrix.data[2];
    model.data[4] = matrix.data[3];
    model.data[5] = matrix.data[4];
    model.data[6] = matrix.data[5];
    model.data[8] = matrix.data[6];
    model.data[9] = matrix.data[7];
    model.data[10]= matrix.data[8];
};

/* Projections */

/**
* @param fov
* @param aspect
* @param near
* @param far
* @return - Projection Matrix4x4
*/
template <typename T>
constexpr static matrix<T, 4> perspective(float fov, float aspect, float near, float far) {
    matrix4<T> result;
    float tanHalfFov = tan(fov / 2.0f);

    result.data[0] = 1.0f / (aspect * tanHalfFov);
    result.data[5] = 1.0f / tanHalfFov;
    result.data[10]= -(far + near) / (far - near);
    result.data[11]= -1.0f;
    result.data[14]= -(2.0f * far * near) / (far - near);
    return result;
};

/**
* @param left
* @param right
* @param bottom
* @param top
* @param near
* @param far
* @return - Projection Matrix4x4
*/
template <typename T>
constexpr static matrix<T, 4> orthographic(float left, float right, float bottom, float top, float near, float far) {
    matrix4<T> result;

    result.data[0] = 2.0f / (right - left);
    result.data[5] = 2.0f / (top - bottom);
    result.data[10] = -2.0f / (far - near);
    result.data[12] = -(right + left) / (right - left);
    result.data[13] = -(top + bottom) / (top - bottom);
    result.data[14] = -(far + near) / (far - near);
    result.data[15] = 1.0f;
    return result;
};

}

#endif