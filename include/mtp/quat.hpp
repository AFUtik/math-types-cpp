/**
* @file quat.hpp
* @brief MathType File.
* @author AFUtik
* @date 2025-07-04
* @version v1.0.0-dev
* @copyright MIT
*
* @details Quaternion Struct.
*/


#ifndef QUAT_HPP
#define QUAT_HPP

namespace mtp {

struct quat {
    float x, y, z, w;

    quat operator*(const quat& q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        };
    }
};

}

#endif