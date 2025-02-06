
#include "Camera.hpp"

#include "Input.hpp"

#include "PSO_util.hpp"

float initialCameraRotRad[3] = { 0.0f, -0.3f, 0.0f };
float initialCameraPosition[3] = { -25.0, 0.0f, 2.0 };

struct Camera camera;


namespace PCamera {

    float initialCameraRotRad[3] = { 0.0f, -0.3f, 0.0f };
    float initialCameraPosition[3] = { -25.0, 0.0f, 2.0 };





    Camera::Camera()
    {
        eulerAnglesRad.a = ::PCamera::initialCameraRotRad[0];
        eulerAnglesRad.b = ::PCamera::initialCameraRotRad[1];
        eulerAnglesRad.c = ::PCamera::initialCameraRotRad[2];
        cameraPosition.x = ::PCamera::initialCameraPosition[0];
        cameraPosition.y = ::PCamera::initialCameraPosition[1];
        cameraPosition.z = ::PCamera::initialCameraPosition[2];

        for (int i = 0; i < 16; i++)
            perspectiveMatrix16[i] = ::perspectiveMatrix16[i];
        
        setPerspectiveMatrix(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);
    }

    Camera::~Camera()
    {
    }



    void Camera::update() {
        // InputState& inputState_main = *inputState_cam_point;

        float forwardX = cos(eulerAnglesRad.c);
        float forwardY = sin(eulerAnglesRad.c);

        float speed = 0.4f;

        if (PInput::inputState.w)
            translate(forwardX * speed, forwardY * speed, 0.0f);
        if (PInput::inputState.s)
            translate(-forwardX * speed, -forwardY * speed, 0.0f);

        if (PInput::inputState.a)
            translate(-forwardY * speed, forwardX * speed, 0.0f);
        if ((PInput::inputState.a && PInput::inputState.d) && (PInput::inputState.mostRecentADpress == 97)) {
            translate(-forwardY * speed, forwardX * speed, 0.0f);
            // printf("%d\n", Input::inputState_main.mostRecentADpress);
        }
        if (PInput::inputState.d)
            translate(forwardY * speed, -forwardX * speed, 0.0f);
        if ((PInput::inputState.a && PInput::inputState.d) && (PInput::inputState.mostRecentADpress == 100)) {
            translate(forwardY * speed, -forwardX * speed, 0.0f);
            // printf("%d\n", Input::inputState_main.mostRecentADpress);
        }


        if (PInput::inputState.au)
            rotateEulerRad(0.0f, 0.03f, 0.0f);
        if (PInput::inputState.ad)
            rotateEulerRad(0.0f, -0.03f, 0.0f);
        if (PInput::inputState.al)
            rotateEulerRad(0.0f, 0.0f, 0.03f);
        if (PInput::inputState.ar)
            rotateEulerRad(0.0f, 0.0f, -0.03f);


        // printf("mouse + ctrl\n");
    // if(inputState_main.mousePressActive && inputState_main.ctrl){
        if (PInput::inputState.middleMouse) {
            float dx = PInput::inputState.pointerX - PInput::inputState.pointerXLastFrame;
            float dy = PInput::inputState.pointerY - PInput::inputState.pointerYLastFrame;

            PInput::inputState.pointerXLastFrame = PInput::inputState.pointerX;
            PInput::inputState.pointerYLastFrame = PInput::inputState.pointerY;

            // printf("%f\n", dx);
            // printf("%f\n", dy);

            rotateEulerRad(0.0f, 0.0f, -dx * 0.005f);
            rotateEulerRad(0.0f, -dy * 0.005f, 0.0f);

        }

        // cam_rotateEulerRad(0.0f, 0.0f, 0.0f);
        setViewMatrix();
    }

    // Camera* getCurrentCamera_pointer() {
    //     return &camera;
    // }

    float* Camera::getViewMatrix() {
        return viewMatrix;
    }
    float* Camera::getPerspectiveMatrix() {
        return perspectiveMatrix16;
    }




    // Match the near clipping plane to window dimensions
    void Camera::setPerspectiveMatrix(int windowWidth, int windowHeight) {
        float windowHeight_f = (float)windowHeight;
        float windowWidth_f = (float)windowWidth;

        perspectiveMatrix16[0] = ZN / 1.0f;
        // if height is smaller than width, ZN is divided by <1.0, as intened!
        perspectiveMatrix16[5] = ZN / (windowHeight_f / windowWidth_f);
    }



    void Camera::setEulerAnglesRad(float a, float b, float c)
    {
        eulerAnglesRad.a = a;
        eulerAnglesRad.b = b;
        eulerAnglesRad.c = c;
    }
    void Camera::rotateEulerRad(float a, float b, float c) {
        eulerAnglesRad.a += a;
        eulerAnglesRad.b += b;
        eulerAnglesRad.c += c;
    }
    void Camera::setPosition(float x, float y, float z)
    {
        cameraPosition.x = x;
        cameraPosition.y = y;
        cameraPosition.z = z;
    }
    void Camera::translate(float x, float y, float z)
    {
        cameraPosition.x += x;
        cameraPosition.y += y;
        cameraPosition.z += z;
    }

    void Camera::setViewMatrix()
    {

        // VERY IMPORTANT TO ZERO FOR EVERY FRAME
        for (size_t i = 0; i < 16; i++)
        {
            viewMatrix[i] = 0;
        }

        // float tempM[16] = {0};
        float tempX[16] = { 0 };
        float tempY[16] = { 0 };
        float tempZ[16] = { 0 };

        float a = eulerAnglesRad.a;
        float b = eulerAnglesRad.b;
        float c = eulerAnglesRad.c;
        // printf("c = %f \n", a);

        // TRANSLATE
        // Row 1
        viewMatrix[0] = 1;
        viewMatrix[3] = -cameraPosition.x;
        // Row 2
        viewMatrix[5] = 1;
        viewMatrix[7] = -cameraPosition.y;
        // Row 3
        viewMatrix[10] = 1;
        viewMatrix[11] = -cameraPosition.z;
        // Row 4
        viewMatrix[15] = 1;

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
                    result[i * 4 + j] += mat[i * 4 + k] * viewMatrix[k * 4 + j];
                }
            }
        }

        for (int i = 0; i < 16; i++)
        {
            viewMatrix[i] = result[i];
            // printf("%f ", result[i]);
        }
        // printf("\n");
    }


} // END PCAMERA NAMESPACE



// fffffffffffffffffff







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

    if (PInput::inputState.w)
        cam_translate(forwardX * 0.2f, forwardY * 0.2f, 0.0f);
    if (PInput::inputState.s)
        cam_translate(-forwardX * 0.2f, -forwardY * 0.2f, 0.0f);

    if (PInput::inputState.a)
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
    if ((PInput::inputState.a && PInput::inputState.d) && (PInput::inputState.mostRecentADpress == 97)) {
        cam_translate(-forwardY * 0.2f, forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }
    if (PInput::inputState.d)
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
    if ((PInput::inputState.a && PInput::inputState.d) && (PInput::inputState.mostRecentADpress == 100)) {
        cam_translate(forwardY * 0.2f, -forwardX * 0.2f, 0.0f);
        // printf("%d\n", inputState_main.mostRecentADpress);
    }


    if (PInput::inputState.au)
        cam_rotateEulerRad(0.0f, 0.05f, 0.0f);
    if (PInput::inputState.ad)
        cam_rotateEulerRad(0.0f, -0.05f, 0.0f);
    if (PInput::inputState.al)
        cam_rotateEulerRad(0.0f, 0.0f, 0.05f);
    if (PInput::inputState.ar)
        cam_rotateEulerRad(0.0f, 0.0f, -0.05f);


    // printf("mouse + ctrl\n");
// if(inputState_main.mousePressActive && inputState_main.ctrl){
    if (PInput::inputState.middleMouse) {
        float dx = PInput::inputState.pointerX - PInput::inputState.pointerXLastFrame;
        float dy = PInput::inputState.pointerY - PInput::inputState.pointerYLastFrame;

        PInput::inputState.pointerXLastFrame = PInput::inputState.pointerX;
        PInput::inputState.pointerYLastFrame = PInput::inputState.pointerY;

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





