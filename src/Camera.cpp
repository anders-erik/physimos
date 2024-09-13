
#include "Camera.hpp"

#include "Input.hpp"

#include "PSO_util.hpp"

float initialCameraRotRad[3] = { 0.0f, -0.3f, 0.0f };
float initialCameraPosition[3] = { -25.0, 0.0f, 2.0 };


struct Camera camera;


// Global input state
extern struct InputState inputState;



void cam_init() {

    camera.eulerAnglesRad.a = initialCameraRotRad[0];
    camera.eulerAnglesRad.b = initialCameraRotRad[1];
    camera.eulerAnglesRad.c = initialCameraRotRad[2];
    camera.cameraPosition.x = initialCameraPosition[0];
    camera.cameraPosition.y = initialCameraPosition[1];
    camera.cameraPosition.z = initialCameraPosition[2];

    for(int i = 0; i < 16; i++)
        camera.perspectiveMatrix16[i] = perspectiveMatrix16[i];

    cam_setPerspectiveMatrix(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);
}

void cam_UpdateCam(){
    // InputState& inputState_main = *inputState_cam_point;

    float forwardX = cos(camera.eulerAnglesRad.c);
    float forwardY = sin(camera.eulerAnglesRad.c);

    if (inputState.w)
        cam_translate(forwardX * 0.2f, forwardY * 0.2f, 0.0f);
    if (inputState.s)
        cam_translate(-forwardX * 0.2f, -forwardY * 0.2f, 0.0f);

    if (inputState.a)
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
    if ((inputState.a && inputState.d) && (inputState.mostRecentADpress == 97)) {
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }
    if (inputState.d)
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
    if ((inputState.a && inputState.d) && (inputState.mostRecentADpress == 100)) {
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }


    if (inputState.au)
        cam_rotateEulerRad(0.0f, 0.05f, 0.0f);
    if (inputState.ad)
        cam_rotateEulerRad(0.0f, -0.05f, 0.0f);
    if (inputState.al)
        cam_rotateEulerRad(0.0f, 0.0f, 0.05f);
    if (inputState.ar)
        cam_rotateEulerRad(0.0f, 0.0f, -0.05f);


    // printf("mouse + ctrl\n");
// if(inputState_main.mousePressActive && inputState_main.ctrl){
    if (inputState.middleMouse) {
        float dx = inputState.pointerX - inputState.pointerXLastFrame;
        float dy = inputState.pointerY - inputState.pointerYLastFrame;

        inputState.pointerXLastFrame = inputState.pointerX;
        inputState.pointerYLastFrame = inputState.pointerY;

        // printf("%f\n", dx);
        // printf("%f\n", dy);

        cam_rotateEulerRad(0.0f, 0.0f, -dx * 0.005f);
        cam_rotateEulerRad(0.0f, -dy * 0.005f, 0.0f);

    }

    // cam_rotateEulerRad(0.0f, 0.0f, 0.0f);
    cam_setViewMatrix();
}

struct Camera* getCurrentCamera_pointer(){
    return &camera;
}

float* cam_getViewMatrix(){
    return camera.viewMatrix;
}
float* cam_getPerspectiveMatrix(){
    return camera.perspectiveMatrix16;
}




// Match the near clipping plane to window dimensions
void cam_setPerspectiveMatrix(int windowWidth, int windowHeight) {
    float windowHeight_f = (float)windowHeight;
    float windowWidth_f = (float)windowWidth;

    camera.perspectiveMatrix16[0] = ZN / 1.0f;
    // if height is smaller than width, ZN is divided by <1.0, as intened!
    camera.perspectiveMatrix16[5] = ZN / (windowHeight_f / windowWidth_f);
}



void cam_setEulerAnglesRad(float a, float b, float c)
{
    camera.eulerAnglesRad.a = a;
    camera.eulerAnglesRad.b = b;
    camera.eulerAnglesRad.c = c;
}
void cam_rotateEulerRad(float a, float b, float c) {
    camera.eulerAnglesRad.a += a;
    camera.eulerAnglesRad.b += b;
    camera.eulerAnglesRad.c += c;
}
void cam_setPosition(float x, float y, float z)
{
    camera.cameraPosition.x = x;
    camera.cameraPosition.y = y;
    camera.cameraPosition.z = z;
}
void cam_translate(float x, float y, float z)
{
    camera.cameraPosition.x += x;
    camera.cameraPosition.y += y;
    camera.cameraPosition.z += z;
}

void cam_setViewMatrix()
{

    // VERY IMPORTANT TO ZERO FOR EVERY FRAME
    for (size_t i = 0; i < 16; i++)
    {
        camera.viewMatrix[i] = 0;
    }

    // float tempM[16] = {0};
    float tempX[16] = { 0 };
    float tempY[16] = { 0 };
    float tempZ[16] = { 0 };

    float a = camera.eulerAnglesRad.a;
    float b = camera.eulerAnglesRad.b;
    float c = camera.eulerAnglesRad.c;
    // printf("c = %f \n", a);

    // TRANSLATE
    // Row 1
    camera.viewMatrix[0] = 1;
    camera.viewMatrix[3] = -camera.cameraPosition.x;
    // Row 2
    camera.viewMatrix[5] = 1;
    camera.viewMatrix[7] = -camera.cameraPosition.y;
    // Row 3
    camera.viewMatrix[10] = 1;
    camera.viewMatrix[11] = -camera.cameraPosition.z;
    // Row 4
    camera.viewMatrix[15] = 1;

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
                result[i * 4 + j] += mat[i * 4 + k] * camera.viewMatrix[k * 4 + j];
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        camera.viewMatrix[i] = result[i];
        // printf("%f ", result[i]);
    }
    // printf("\n");
}





