#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "vector.hpp"
#include "matrix.hpp"

#include <cmath>

namespace mtp {

template<typename T>
struct transform {
    matrix4<T> mm = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }; /* Defualt Model Matrix */
};

template<typename T>
struct transform2 : public transform<T> {
    vector2<T*> position = {&this->mm[12], &this->mm[13]};
    matrix2<T*> rot_mat  = {&this->mm[0], &this->mm[1],
                            &this->mm[4], &this->mm[5]};               
    vector2<T*> scale    = {&this->mm[3], &this->mm[7]};
};


template<typename T>
struct transform3 : public transform<T> {
    vector3<T*> position = {&this->mm[12], &this->mm[13], &this->mm[14]};
    matrix3<T*> rot_mat  = {&this->mm[0], &this->mm[1], &this->mm[2],
                            &this->mm[4], &this->mm[5], &this->mm[6],
                            &this->mm[8], &this->mm[9], &this->mm[10]};               
    vector3<T*> scale    = {&this->mm[3], &this->mm[7], &this->mm[11]};
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
    float ca = std::cos(vector.x), sa = std::sin(vector.x);
    float cb = std::cos(vector.y), sb = std::sin(vector.y);
    float cg = std::cos(vector.z), sg = std::sin(vector.z);

    /* Matrix Multiplications - R_x * R_y * R_z */
    model[0] = cb * cg;
    model[1] = cg * sa * sb - sg * ca;
    model[2] = cg * ca * sb + sg * sa;

    model[4] = sb;
    model[5] = cb * ca;
    model[6] = -cb * sa;

    model[8] = -sg * cb;
    model[9] = sg * sa * sb + cg * ca;
    model[10]= sg * ca * sb - cg * sa;
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

template <typename T>
static constexpr inline void setRotation(matrix<T, 3, 3> &model, const matrix<T, 2, 2> &matrix) {
    model.data[0] = matrix.data[0];
    model.data[1] = matrix.data[1];
    model.data[3] = matrix.data[2];
    model.data[4] = matrix.data[3];
};

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

using transform2d = transform2<double>;
using transform3d = transform3<double>;
using transform2f = transform2<float>;
using transform3f = transform3<float>;

}

#endif