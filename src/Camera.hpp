

#include <math.h>
#include <stdio.h>

#include "Types.hpp"

// settings
#define ZF 300.0f // far plane
#define ZN 1.0f // near plane 

float initialCameraRotRad[3] = { 0.0f, 0.0f, 0.0f };
float initialCameraPosition[3] = { -10.0, 0, 0.0 };

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
    ZN, 0, 0, 0,
    0, ZN, 0, 0,
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
    Camera();

    void setEulerAnglesRad(float a, float b, float c);
    void rotateEulerRad(float a, float b, float c);
    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);

    void setViewMatrix();
    void multiplyViewMatrix(float* mat);
};

//
//
//  DEFINITIONS
//
//



void Camera::setEulerAnglesRad(float a, float b, float c)
{
    this->eulerAnglesRad.a = a;
    this->eulerAnglesRad.b = b;
    this->eulerAnglesRad.c = c;
}
void Camera::rotateEulerRad(float a, float b, float c){
    this->eulerAnglesRad.a += a;
    this->eulerAnglesRad.b += b;
    this->eulerAnglesRad.c += c;
}
void Camera::setPosition(float x, float y, float z)
{
    this->cameraPosition.x = x;
    this->cameraPosition.y = y;
    this->cameraPosition.z = z;
}
void Camera::translate(float x, float y, float z)
{
    this->cameraPosition.x += x;
    this->cameraPosition.y += y;
    this->cameraPosition.z += z;
}

void Camera::setViewMatrix()
{

    // VERY IMPORTANT TO ZERO FOR EVERY FRAME
    for (size_t i = 0; i < 16; i++)
    {
        this->viewMatrix[i] = 0;
    }

    // float tempM[16] = {0};
    float tempX[16] = { 0 };
    float tempY[16] = { 0 };
    float tempZ[16] = { 0 };

    float a = this->eulerAnglesRad.a;
    float b = this->eulerAnglesRad.b;
    float c = this->eulerAnglesRad.c;
    // printf("c = %f \n", a);

    // TRANSLATE
    // Row 1
    this->viewMatrix[0] = 1;
    this->viewMatrix[3] = -this->cameraPosition.x;
    // Row 2
    this->viewMatrix[5] = 1;
    this->viewMatrix[7] = -this->cameraPosition.y;
    // Row 3
    this->viewMatrix[10] = 1;
    this->viewMatrix[11] = -this->cameraPosition.z;
    // Row 4
    this->viewMatrix[15] = 1;

    // rotate X
    // Row 1
    tempX[0] = 1.0f;
    // Row 2
    tempX[5] = cos(a);
    tempX[6] = sin(a);
    // Row 3
    tempX[9] = -sin(a);
    tempX[10] = cos(a);
    // Row 4
    tempX[15] = 1.0f;

    // rotate Y
    // Row 1
    tempY[0] = cos(b);
    tempY[2] = sin(b);
    // Row 2
    tempY[5] = 1.0f;
    // Row 3
    tempY[8] = -sin(b);
    tempY[10] = cos(b);
    // Row 4
    tempY[15] = 1.0f;

    // rotate Z
    // Row 1
    tempZ[0] = cos(c);
    tempZ[1] = sin(c);
    // Row 2
    tempZ[4] = -sin(c);
    tempZ[5] = cos(c);
    // Row 3
    tempZ[10] = 1.0f;
    // Row 4
    tempZ[15] = 1.0f;

    // multiply matrices : X x Y x Z x T
    multiplyViewMatrix(tempZ);
    multiplyViewMatrix(tempY);
    multiplyViewMatrix(tempX);

    // printf("\n");printf("\n");

    // RESET VIEWMATRIX FOR TESTING - 2024-08-26
    // THIS WORKS!
    // for (size_t i = 0; i < 16; i++)
    // {
    //     this->viewMatrix[i] = 0;
    // }
    // this->viewMatrix[0] = cos(c);
    // this->viewMatrix[1] = sin(c);

    // this->viewMatrix[4] = -sin(c);
    // this->viewMatrix[5] = cos(c);
    // // this->viewMatrix[5] = 1.0f;

    // this->viewMatrix[10] = 1.0f;

    // this->viewMatrix[15] = 1.0f;
}

void Camera::multiplyViewMatrix(float* mat)
{

    float result[16] = { 0 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result[i * 4 + j] += mat[i * 4 + k] * this->viewMatrix[k * 4 + j];
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        this->viewMatrix[i] = result[i];
        // printf("%f ", result[i]);
    }
    // printf("\n");
}

Camera::Camera()
{
    // eulerAnglesRad.a(11);
    this->eulerAnglesRad.a = initialCameraRotRad[0];
    this->eulerAnglesRad.b = initialCameraRotRad[1];
    this->eulerAnglesRad.c = initialCameraRotRad[2];
    this->cameraPosition.x = initialCameraPosition[0];
    this->cameraPosition.y = initialCameraPosition[1];
    this->cameraPosition.z = initialCameraPosition[2];
}
