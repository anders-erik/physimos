
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include "pobject.hh"

#include "shader.hpp"

#include "resources.hh"

#include "transform.hh"



const float sanityMatrix16[16] = {
    0, -1, 0, 0,
    0, 0, 1, 0,
    -1, 0, 0, 0,
    0, 0, 0, 1,
};



PObject::PObject(std::string _modelName, std::string _objectName) {

    name = _objectName;
    // LoadWorldObject(path);

    model_ptr = new objects::Model(_modelName);
    transform = new Transform();
    transform_0 = new Transform();


    // setVaoVbo_obj();
    // worldCube4_obj.setShaderProgram(&worldShader);
    // setShaderProgram(&worldObjShader);

    if(model_ptr->loadedVertStructure == res::ModelVertStucture::p3n3t2){
        shader = getShader(Shaders::worldObj);
        shaderType = Shaders::worldObj;
    }
    else if (model_ptr->loadedVertStructure == res::ModelVertStucture::p3c3){
        shader = getShader(Shaders::world);
        shaderType = Shaders::world;
    }
    else if (model_ptr->loadedVertStructure == res::ModelVertStucture::p3c3t2){
        shader = getShader(Shaders::world);
        shaderType = Shaders::world;
    }
    else if (model_ptr->loadedVertStructure == res::ModelVertStucture::p3) {
        shader = getShader(Shaders::worldWireframe);
        shaderType = Shaders::worldWireframe;
    }
}

/**
 *  Adds the values of mat2 to mat1
 */
void add_mat16(float* mat1, float* mat2){
    for(int i = 0; i<15; i++){ // ignore the last value 
        mat1[i] += mat2[i];
    }
}


void PObject::update() {
    // std::cout << "updating : " << name << std::endl;
    // if(name == "transform_1")

    // Set model transform before calling children, as the children will use it!
    if(parent == nullptr){
        SetModelMatrixRowMajor();
    }
    else{
        SetModelMatrixRowMajor_withParent();
    }   

    for(PObject* _wo : children){
        _wo->update();
    }

}



void PObject::toggleWireframe(){
    // set wireframe
    if (shaderType == Shaders::world || shaderType == Shaders::worldObj){
        shader = getShader(Shaders::worldWireframe);
        shaderType = Shaders::worldWireframe;
    }
    // Set regular renderers
    else if (shaderType == Shaders::worldWireframe){
        if (model_ptr->modelFileType == res::ModelFileType::obj){
            shader = getShader(Shaders::worldObj);
            shaderType = Shaders::worldObj;
        }
        else if (model_ptr->modelFileType == res::ModelFileType::pso){
            shader = getShader(Shaders::world);
            shaderType = Shaders::world;
        }
    }
}









void PObject::printTransformMatrix() {
    std::cout << this->modelMatrixRowMajor[0] << " " << this->modelMatrixRowMajor[1] << " " << this->modelMatrixRowMajor[2] << " " << this->modelMatrixRowMajor[3] << std::endl;
    std::cout << this->modelMatrixRowMajor[4] << " " << this->modelMatrixRowMajor[5] << " " << this->modelMatrixRowMajor[6] << " " << this->modelMatrixRowMajor[7] << std::endl;
    std::cout << this->modelMatrixRowMajor[8] << " " << this->modelMatrixRowMajor[9] << " " << this->modelMatrixRowMajor[10] << " " << this->modelMatrixRowMajor[11] << std::endl;
    std::cout << this->modelMatrixRowMajor[12] << " " << this->modelMatrixRowMajor[13] << " " << this->modelMatrixRowMajor[14] << " " << this->modelMatrixRowMajor[15] << std::endl;
}





void PObject::SetModelMatrixRowMajor() {
    // this->transformMatrixRowMajor
    float* scaleVecData = transform->scale.get_fdata();
    float Sx = scaleVecData[0];
    float Sy = scaleVecData[1];
    float Sz = scaleVecData[2];

    float Tx = transform->position.data[0];
    float Ty = transform->position.data[1];
    float Tz = transform->position.data[2];

    // // Row 1
    // this->transformMatrixRowMajor[0] = Sx;
    // this->transformMatrixRowMajor[1] = 0;
    // this->transformMatrixRowMajor[2] = 0;
    // this->transformMatrixRowMajor[3] = Tx;

    // // Row 2
    // this->transformMatrixRowMajor[4] = 0;
    // this->transformMatrixRowMajor[5] = Sy;
    // this->transformMatrixRowMajor[6] = 0;
    // this->transformMatrixRowMajor[7] = Ty;


    // // Row 3
    // this->transformMatrixRowMajor[8] = 0;
    // this->transformMatrixRowMajor[9] = 0;
    // this->transformMatrixRowMajor[10] = Sz;
    // this->transformMatrixRowMajor[11] = Tz;


    // // Row 4
    // this->transformMatrixRowMajor[12] = 0;
    // this->transformMatrixRowMajor[13] = 0;
    // this->transformMatrixRowMajor[14] = 0;
    // this->transformMatrixRowMajor[15] = 1;


    float c_ph = cos(transform->rotation.data[0]);
    float s_ph = sin(transform->rotation.data[0]);
    float c_th = cos(transform->rotation.data[1]);
    float s_th = sin(transform->rotation.data[1]);
    float c_ps = cos(transform->rotation.data[2]);
    float s_ps = sin(transform->rotation.data[2]);

    float a11 = c_ps * c_ph - s_ps * c_th * s_ph;
    float a12 = -c_ps * s_ph - s_ps * c_th * c_ph;
    float a13 = s_ps * s_th;
    float a14 = 0;

    float a21 = s_ps * c_ph + c_ps * c_th * s_ph;
    float a22 = -s_ps * s_ph + c_ps * c_th * c_ph;
    float a23 = -c_ps * s_th;
    float a24 = 0;

    float a31 = s_th * s_ph;
    float a32 = s_th * c_ph;
    float a33 = c_th;
    float a34 = 0;

    float a41 = 0;
    float a42 = 0;
    float a43 = 0;
    float a44 = 1;


    // Row 1
    this->modelMatrixRowMajor[0] = Sx * (a11 + Tx * a41);
    this->modelMatrixRowMajor[1] = Sy * (a12 + Tx * a42);
    this->modelMatrixRowMajor[2] = Sz * (a13 + Tx * a43);
    this->modelMatrixRowMajor[3] = a14 + Tx * a44;

    // Row 2
    this->modelMatrixRowMajor[4] = Sx * (a21 + Ty * a41);
    this->modelMatrixRowMajor[5] = Sy * (a22 + Ty * a42);
    this->modelMatrixRowMajor[6] = Sz * (a23 + Ty * a43);
    this->modelMatrixRowMajor[7] = a24 + Ty * a44;;

    // Row 3
    this->modelMatrixRowMajor[8] = Sx * (a31 + Tz * a41);
    this->modelMatrixRowMajor[9] = Sy * (a32 + Tz * a42);
    this->modelMatrixRowMajor[10] = Sz * (a33 + Tz * a43);
    this->modelMatrixRowMajor[11] = a34 + Tz * a44;;

    // Row 4
    this->modelMatrixRowMajor[12] = Sx * a41;
    this->modelMatrixRowMajor[13] = Sy * a42;
    this->modelMatrixRowMajor[14] = Sx * a43;
    this->modelMatrixRowMajor[15] = a44;

}

void PObject::SetModelMatrixRowMajor_withParent() {
    // this->transformMatrixRowMajor
    float* scaleVecData = transform->scale.get_fdata();
    float Sx = scaleVecData[0];
    float Sy = scaleVecData[1];
    float Sz = scaleVecData[2];

    float Tx = transform->position.data[0] + parent->transform->position.data[0];
    float Ty = transform->position.data[1] + parent->transform->position.data[1];
    float Tz = transform->position.data[2] + parent->transform->position.data[2];


    float c_ph = cos(transform->rotation.data[0] + parent->transform->rotation.data[0]);
    float s_ph = sin(transform->rotation.data[0] + parent->transform->rotation.data[0]);
    float c_th = cos(transform->rotation.data[1] + parent->transform->rotation.data[1]);
    float s_th = sin(transform->rotation.data[1] + parent->transform->rotation.data[1]);
    float c_ps = cos(transform->rotation.data[2] + parent->transform->rotation.data[2]);
    float s_ps = sin(transform->rotation.data[2] + parent->transform->rotation.data[2]);

    float a11 = c_ps * c_ph - s_ps * c_th * s_ph;
    float a12 = -c_ps * s_ph - s_ps * c_th * c_ph;
    float a13 = s_ps * s_th;
    float a14 = 0;

    float a21 = s_ps * c_ph + c_ps * c_th * s_ph;
    float a22 = -s_ps * s_ph + c_ps * c_th * c_ph;
    float a23 = -c_ps * s_th;
    float a24 = 0;

    float a31 = s_th * s_ph;
    float a32 = s_th * c_ph;
    float a33 = c_th;
    float a34 = 0;

    float a41 = 0;
    float a42 = 0;
    float a43 = 0;
    float a44 = 1;


    // Row 1
    this->modelMatrixRowMajor[0] = Sx * (a11 + Tx * a41);
    this->modelMatrixRowMajor[1] = Sy * (a12 + Tx * a42);
    this->modelMatrixRowMajor[2] = Sz * (a13 + Tx * a43);
    this->modelMatrixRowMajor[3] = a14 + Tx * a44;

    // Row 2
    this->modelMatrixRowMajor[4] = Sx * (a21 + Ty * a41);
    this->modelMatrixRowMajor[5] = Sy * (a22 + Ty * a42);
    this->modelMatrixRowMajor[6] = Sz * (a23 + Ty * a43);
    this->modelMatrixRowMajor[7] = a24 + Ty * a44;;

    // Row 3
    this->modelMatrixRowMajor[8] = Sx * (a31 + Tz * a41);
    this->modelMatrixRowMajor[9] = Sy * (a32 + Tz * a42);
    this->modelMatrixRowMajor[10] = Sz * (a33 + Tz * a43);
    this->modelMatrixRowMajor[11] = a34 + Tz * a44;;

    // Row 4
    this->modelMatrixRowMajor[12] = Sx * a41;
    this->modelMatrixRowMajor[13] = Sy * a42;
    this->modelMatrixRowMajor[14] = Sx * a43;
    this->modelMatrixRowMajor[15] = a44;

}






