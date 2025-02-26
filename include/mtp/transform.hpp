#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "vector.hpp"
#include "matrix.hpp"

namespace mtp {

template<typename T>
struct transform {
    matrix4<T> mm;/* Model Matrix */
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

using transform2d = transform2<double>;
using transform3d = transform3<double>;
using transform2f = transform2<float>;
using transform3f = transform3<float>;

}

#endif