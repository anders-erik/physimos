
#include "Camera.hpp"

#include "Input.hpp"

float initialCameraRotRad[3] = { 0.0f, 0.0f, 0.0f };
float initialCameraPosition[3] = { -10.0, 0, 0.0 };


struct Camera__ camera__;
 
// Point 'inputState_main' to global InputState object
InputState* inputState_cam_point;
// InputState& inputState_cam;

void initCamera(){
    inputState_cam_point = getCurrentInputStatePointer();
    // inputState_cam = *inputState_cam_point;


    camera__.eulerAnglesRad.a = initialCameraRotRad[0];
    camera__.eulerAnglesRad.b = initialCameraRotRad[1];
    camera__.eulerAnglesRad.c = initialCameraRotRad[2];
    camera__.cameraPosition.x = initialCameraPosition[0];
    camera__.cameraPosition.y = initialCameraPosition[1];
    camera__.cameraPosition.z = initialCameraPosition[2];

    for(int i = 0; i < 16; i++)
        camera__.perspectiveMatrix16[i] = perspectiveMatrix16[i];
}

void cam_UpdateCam(){
    InputState& inputState_main = *inputState_cam_point;

    float forwardX = cos(camera__.eulerAnglesRad.c);
    float forwardY = sin(camera__.eulerAnglesRad.c);

    if (inputState_main.w)
        cam_translate(forwardX * 0.2f, forwardY * 0.2f, 0.0f);
    if (inputState_main.s)
        cam_translate(-forwardX * 0.2f, -forwardY * 0.2f, 0.0f);

    if (inputState_main.a)
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
    if ((inputState_main.a && inputState_main.d) && (inputState_main.mostRecentADpress == 97)) {
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }
    if (inputState_main.d)
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
    if ((inputState_main.a && inputState_main.d) && (inputState_main.mostRecentADpress == 100)) {
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }


    if (inputState_main.au)
        cam_rotateEulerRad(0.0f, 0.05f, 0.0f);
    if (inputState_main.ad)
        cam_rotateEulerRad(0.0f, -0.05f, 0.0f);
    if (inputState_main.al)
        cam_rotateEulerRad(0.0f, 0.0f, 0.05f);
    if (inputState_main.ar)
        cam_rotateEulerRad(0.0f, 0.0f, -0.05f);


    // printf("mouse + ctrl\n");
// if(inputState_main.mousePressActive && inputState_main.ctrl){
    if (inputState_main.middleMouse) {
        float dx = inputState_main.pointerX - inputState_main.pointerXLastFrame;
        float dy = inputState_main.pointerY - inputState_main.pointerYLastFrame;

        inputState_main.pointerXLastFrame = inputState_main.pointerX;
        inputState_main.pointerYLastFrame = inputState_main.pointerY;

        // printf("%f\n", dx);
        // printf("%f\n", dy);

        cam_rotateEulerRad(0.0f, 0.0f, -dx * 0.005f);
        cam_rotateEulerRad(0.0f, -dy * 0.005f, 0.0f);

    }

    cam_rotateEulerRad(0.0f, 0.0f, 0.0f);
    cam_setViewMatrix();
}

struct Camera__* getCurrentCamera_pointer(){
    return &camera__;
}

float* cam_getViewMatrix(){
    return camera__.viewMatrix;
}
float* cam_getPerspectiveMatrix(){
    return camera__.perspectiveMatrix16;
}




// Match the near clipping plane to window dimensions
void cam_setPerspectiveMatrix(int windowWidth, int windowHeight) {
    float windowHeight_f = (float)windowHeight;
    float windowWidth_f = (float)windowWidth;

    camera__.perspectiveMatrix16[0] = ZN / 1.0f;
    // if height is smaller than width, ZN is divided by <1.0, as intened!
    camera__.perspectiveMatrix16[5] = ZN / (windowHeight_f / windowWidth_f);
}



void cam_setEulerAnglesRad(float a, float b, float c)
{
    camera__.eulerAnglesRad.a = a;
    camera__.eulerAnglesRad.b = b;
    camera__.eulerAnglesRad.c = c;
}
void cam_rotateEulerRad(float a, float b, float c) {
    camera__.eulerAnglesRad.a += a;
    camera__.eulerAnglesRad.b += b;
    camera__.eulerAnglesRad.c += c;
}
void cam_setPosition(float x, float y, float z)
{
    camera__.cameraPosition.x = x;
    camera__.cameraPosition.y = y;
    camera__.cameraPosition.z = z;
}
void cam_translate(float x, float y, float z)
{
    camera__.cameraPosition.x += x;
    camera__.cameraPosition.y += y;
    camera__.cameraPosition.z += z;
}

void cam_setViewMatrix()
{

    // VERY IMPORTANT TO ZERO FOR EVERY FRAME
    for (size_t i = 0; i < 16; i++)
    {
        camera__.viewMatrix[i] = 0;
    }

    // float tempM[16] = {0};
    float tempX[16] = { 0 };
    float tempY[16] = { 0 };
    float tempZ[16] = { 0 };

    float a = camera__.eulerAnglesRad.a;
    float b = camera__.eulerAnglesRad.b;
    float c = camera__.eulerAnglesRad.c;
    // printf("c = %f \n", a);

    // TRANSLATE
    // Row 1
    camera__.viewMatrix[0] = 1;
    camera__.viewMatrix[3] = -camera__.cameraPosition.x;
    // Row 2
    camera__.viewMatrix[5] = 1;
    camera__.viewMatrix[7] = -camera__.cameraPosition.y;
    // Row 3
    camera__.viewMatrix[10] = 1;
    camera__.viewMatrix[11] = -camera__.cameraPosition.z;
    // Row 4
    camera__.viewMatrix[15] = 1;

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
    cam_multiplyViewMatrix(tempZ);
    cam_multiplyViewMatrix(tempY);
    cam_multiplyViewMatrix(tempX);

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

void cam_multiplyViewMatrix(float* mat)
{

    float result[16] = { 0 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result[i * 4 + j] += mat[i * 4 + k] * camera__.viewMatrix[k * 4 + j];
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        camera__.viewMatrix[i] = result[i];
        // printf("%f ", result[i]);
    }
    // printf("\n");
}








// -------------------------------------







// // Match the near clipping plane to window dimensions
// void Camera::setPerspectiveMatrix(int windowWidth, int windowHeight){
//     float windowHeight_f = (float) windowHeight;
//     float windowWidth_f = (float)windowWidth;
    
//     perspectiveMatrix16[0] = ZN / 1.0f;
//     // if height is smaller than width, ZN is divided by <1.0, as intened!
//     perspectiveMatrix16[5] = ZN / (windowHeight_f / windowWidth_f);
// }



// void Camera::setEulerAnglesRad(float a, float b, float c)
// {
//     this->eulerAnglesRad.a = a;
//     this->eulerAnglesRad.b = b;
//     this->eulerAnglesRad.c = c;
// }
// void Camera::rotateEulerRad(float a, float b, float c){
//     this->eulerAnglesRad.a += a;
//     this->eulerAnglesRad.b += b;
//     this->eulerAnglesRad.c += c;
// }
// void Camera::setPosition(float x, float y, float z)
// {
//     this->cameraPosition.x = x;
//     this->cameraPosition.y = y;
//     this->cameraPosition.z = z;
// }
// void Camera::translate(float x, float y, float z)
// {
//     this->cameraPosition.x += x;
//     this->cameraPosition.y += y;
//     this->cameraPosition.z += z;
// }

// void Camera::setViewMatrix()
// {

//     // VERY IMPORTANT TO ZERO FOR EVERY FRAME
//     for (size_t i = 0; i < 16; i++)
//     {
//         this->viewMatrix[i] = 0;
//     }

//     // float tempM[16] = {0};
//     float tempX[16] = { 0 };
//     float tempY[16] = { 0 };
//     float tempZ[16] = { 0 };

//     float a = this->eulerAnglesRad.a;
//     float b = this->eulerAnglesRad.b;
//     float c = this->eulerAnglesRad.c;
//     // printf("c = %f \n", a);

//     // TRANSLATE
//     // Row 1
//     this->viewMatrix[0] = 1;
//     this->viewMatrix[3] = -this->cameraPosition.x;
//     // Row 2
//     this->viewMatrix[5] = 1;
//     this->viewMatrix[7] = -this->cameraPosition.y;
//     // Row 3
//     this->viewMatrix[10] = 1;
//     this->viewMatrix[11] = -this->cameraPosition.z;
//     // Row 4
//     this->viewMatrix[15] = 1;

//     // rotate X
//     // Row 1
//     tempX[0] = 1.0f;
//     // Row 2
//     tempX[5] = cos(a);
//     tempX[6] = sin(a);
//     // Row 3
//     tempX[9] = -sin(a);
//     tempX[10] = cos(a);
//     // Row 4
//     tempX[15] = 1.0f;

//     // rotate Y
//     // Row 1
//     tempY[0] = cos(b);
//     tempY[2] = sin(b);
//     // Row 2
//     tempY[5] = 1.0f;
//     // Row 3
//     tempY[8] = -sin(b);
//     tempY[10] = cos(b);
//     // Row 4
//     tempY[15] = 1.0f;

//     // rotate Z
//     // Row 1
//     tempZ[0] = cos(c);
//     tempZ[1] = sin(c);
//     // Row 2
//     tempZ[4] = -sin(c);
//     tempZ[5] = cos(c);
//     // Row 3
//     tempZ[10] = 1.0f;
//     // Row 4
//     tempZ[15] = 1.0f;

//     // multiply matrices : X x Y x Z x T
//     multiplyViewMatrix(tempZ);
//     multiplyViewMatrix(tempY);
//     multiplyViewMatrix(tempX);

//     // printf("\n");printf("\n");

//     // RESET VIEWMATRIX FOR TESTING - 2024-08-26
//     // THIS WORKS!
//     // for (size_t i = 0; i < 16; i++)
//     // {
//     //     this->viewMatrix[i] = 0;
//     // }
//     // this->viewMatrix[0] = cos(c);
//     // this->viewMatrix[1] = sin(c);

//     // this->viewMatrix[4] = -sin(c);
//     // this->viewMatrix[5] = cos(c);
//     // // this->viewMatrix[5] = 1.0f;

//     // this->viewMatrix[10] = 1.0f;

//     // this->viewMatrix[15] = 1.0f;
// }

// void Camera::multiplyViewMatrix(float* mat)
// {

//     float result[16] = { 0 };

//     for (int i = 0; i < 4; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             for (int k = 0; k < 4; k++)
//             {
//                 result[i * 4 + j] += mat[i * 4 + k] * this->viewMatrix[k * 4 + j];
//             }
//         }
//     }

//     for (int i = 0; i < 16; i++)
//     {
//         this->viewMatrix[i] = result[i];
//         // printf("%f ", result[i]);
//     }
//     // printf("\n");
// }

// Camera::Camera()
// {
//     // eulerAnglesRad.a(11);
//     this->eulerAnglesRad.a = initialCameraRotRad[0];
//     this->eulerAnglesRad.b = initialCameraRotRad[1];
//     this->eulerAnglesRad.c = initialCameraRotRad[2];
//     this->cameraPosition.x = initialCameraPosition[0];
//     this->cameraPosition.y = initialCameraPosition[1];
//     this->cameraPosition.z = initialCameraPosition[2];
// }

