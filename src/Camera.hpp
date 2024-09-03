#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>
#include <stdio.h>

#include "Types.hpp"



// settings 
#define ZF 300.0f // far plane
#define ZN 1.0f // near plane 


// const float viewMatrix16[16] = {
// 	1, 0, 0, 10,
// 	0, 1, 0, 0,
// 	0, 0, 1, -5,
// 	0, 0, 0, 1,
// };

const float perspectiveMatrix16_[16] = {
    ZN, 0, 0, 0,
    0, ZN, 0, 0,
    0, 0, -1.0f / (ZF - ZN), ZN / (ZF - ZN),
    0, 0, 1.0f, 0,
};
// http://www.songho.ca/opengl/gl_projectionmatrix.html
const float perspectiveMatrix16[16] = {
    ZN / 1.0f, 0, 0, 0,
    0, ZN / 1.0f, 0, 0,
    0, 0, -(ZF + ZN) / (ZF - ZN), -2 * ZN * ZF / (ZF - ZN),
    0, 0, -1.0f, 0,
};

class Camera
{
private:
public:
    struct EulerAnglesRad eulerAnglesRad;
    struct Point3 cameraPosition;
    float viewMatrix[16];
    float perspectiveMatrix16[16] = {    
        ZN / 1.0f, 0, 0, 0,
        0, ZN / 1.0f, 0, 0,
        0, 0, -(ZF + ZN) / (ZF - ZN), -2 * ZN * ZF / (ZF - ZN),
        0, 0, -1.0f, 0,
    };
    Camera();

    void setEulerAnglesRad(float a, float b, float c);
    void rotateEulerRad(float a, float b, float c);
    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);

    void setViewMatrix();
    void setPerspectiveMatrix(int windowWidth, int windowHeight);
    void multiplyViewMatrix(float* mat);
};

#endif