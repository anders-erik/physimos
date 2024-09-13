#ifndef VECTOR_MATH_HPP
#define VECTOR_MATH_HPP


#include "Types.hpp"

#include "VectorMath.hpp"



float* mul_vec3_mat16(float* _vec3, float* _mat16) {
    float _tempVec3[3] = {0.0f, 0.0f, 0.0f};

    _tempVec3[0] += _mat16[0] * _vec3[0] + _mat16[1] * _vec3[1] + _mat16[2] * _vec3[2] + _mat16[3] * 1.0f;
    _tempVec3[1] += _mat16[4] * _vec3[0] + _mat16[5] * _vec3[1] + _mat16[6] * _vec3[2] + _mat16[7] * 1.0f;
    _tempVec3[2] += _mat16[8] * _vec3[0] + _mat16[9] * _vec3[1] + _mat16[10] * _vec3[2] + _mat16[11] * 1.0f;
    // 1.0f += _mat16[12] * _vec3[0] + _mat16[13] * _vec3[1] + _mat16[14] * _vec3[2] + _mat16[15] * 1.0f;


    _vec3[0] = _tempVec3[0];
    _vec3[1] = _tempVec3[1];
    _vec3[2] = _tempVec3[2];

    return _vec3;
}

#endif