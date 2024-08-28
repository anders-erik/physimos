#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>


#include "Types.hpp"

class WorldObject {

public:
    WorldObject(const char* path);
    WorldObject();
    

    std::vector<float> vertices;
	int vertexCount;

	Vec3 rotation = {0.0f, 0.0f, 0.0f};
	Vec3 position = {0.0f, 0.0f, 0.0f};
	Vec3 scale = {1.0f, 1.0f, 1.0f};
	float transformMatrixRowMajor[16] = {0};

    std::string modelPath = "";




    Vec3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    Vec3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
	
	Vec3 position_0;
	Vec3 velocity_0;



    // MATH
    void Rotate(Vec3 rotationVector);
    void Translate(Vec3 moveVector);
    void SetScale(Vec3 scaleVector);
    void SetTransformMatrixRowMajor();
    

    void loadVerticexFromFile(); // Not currently in use !
    void printVertices();

    void printPosition();
    void printTransformMatrix();
    
};

void WorldObject::printPosition() {
    std::cout << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
}

void WorldObject::printTransformMatrix() {
    std::cout << this->transformMatrixRowMajor[0] << " " << this->transformMatrixRowMajor[1] << " " << this->transformMatrixRowMajor[2] << " " << this->transformMatrixRowMajor[3] << std::endl;
    std::cout << this->transformMatrixRowMajor[4] << " " << this->transformMatrixRowMajor[5] << " " << this->transformMatrixRowMajor[6] << " " << this->transformMatrixRowMajor[7] << std::endl;
    std::cout << this->transformMatrixRowMajor[8] << " " << this->transformMatrixRowMajor[9] << " " << this->transformMatrixRowMajor[10] << " " << this->transformMatrixRowMajor[11] << std::endl;
    std::cout << this->transformMatrixRowMajor[12] << " " << this->transformMatrixRowMajor[13] << " " << this->transformMatrixRowMajor[14] << " " << this->transformMatrixRowMajor[15] << std::endl;
}




void WorldObject::Rotate(Vec3 rotationVector) {
    this->rotation.x += rotationVector.x;
    this->rotation.y += rotationVector.y;
    this->rotation.z += rotationVector.z;
}

void WorldObject::Translate(Vec3 moveVector) {
    this->position.x += moveVector.x;
    this->position.y += moveVector.y;
    this->position.z += moveVector.z;
}

void WorldObject::SetScale(Vec3 scaleVector) {
    this->scale.x = scaleVector.x;
    this->scale.y = scaleVector.y;
    this->scale.z = scaleVector.z;
}

void WorldObject::SetTransformMatrixRowMajor(){
    // this->transformMatrixRowMajor
    float Sx = this->scale.x;
    float Sy = this->scale.y;
    float Sz = this->scale.z;

    float Tx = this->position.x;
    float Ty = this->position.y;
    float Tz = this->position.z;

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


    float c_ph = cos(this->rotation.x);
    float s_ph = sin(this->rotation.x);
    float c_th = cos(this->rotation.y);
    float s_th = sin(this->rotation.y);
    float c_ps = cos(this->rotation.z);
    float s_ps = sin(this->rotation.z);

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
    this->transformMatrixRowMajor[0] = Sx * (a11 + Tx * a41);
    this->transformMatrixRowMajor[1] = Sy * (a12 + Tx * a42);
    this->transformMatrixRowMajor[2] = Sz * (a13 + Tx * a43);
    this->transformMatrixRowMajor[3] = a14 + Tx * a44;

    // Row 2
    this->transformMatrixRowMajor[4] = Sx * (a21 + Ty * a41);
    this->transformMatrixRowMajor[5] = Sy * (a22 + Ty * a42);
    this->transformMatrixRowMajor[6] = Sz * (a23 + Ty * a43);
    this->transformMatrixRowMajor[7] = a24 + Ty * a44;;

    // Row 3
    this->transformMatrixRowMajor[8] = Sx * (a31 + Tz * a41);
    this->transformMatrixRowMajor[9] = Sy * (a32 + Tz * a42);
    this->transformMatrixRowMajor[10] = Sz * (a33 + Tz * a43);
    this->transformMatrixRowMajor[11] = a34 + Tz * a44;;

    // Row 4
    this->transformMatrixRowMajor[12] = Sx * a41;
    this->transformMatrixRowMajor[13] = Sy * a42;
    this->transformMatrixRowMajor[14] = Sx * a43;
    this->transformMatrixRowMajor[15] = a44;

}

WorldObject::WorldObject(){

}

WorldObject::WorldObject(const char* path) {

    std::cout << std::endl << "Loading model: " << path << ". ";
    this->modelPath = path;

    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelString;

    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {

        modelFile.open(path);


        modelStream << modelFile.rdbuf();

        modelString = modelStream.str();

        modelFile.close();

        std::string item;
        float number;

        // Stepping through each comma-separated value
        while (std::getline(modelStream, item, ',')) {
            // Remove any leading or trailing whitespace from the item
            std::stringstream itemStream(item);
            itemStream >> number;

            // Add the parsed float to the vector
            this->vertices.push_back(number);
        }

        std::cout << "OK.  [" << this->vertices.size() << " values]" << std::endl << std::endl;

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " EROOR. [" << this->vertices.size() << " values]" << std::endl;
        std::cout << "ERROR::READING_PSO_FILE" << e.what() << std::endl;
    }

    

    // std::cout << modelString;



    

}

void WorldObject::loadVerticexFromFile() {

    std::cout << "Reading model at path : " << &this->modelPath << std::endl;

}

void WorldObject::printVertices() {
    // Check contents
    std::cout << "Values stored in the vector: \n";
    int i = 0;
    for (float value : this->vertices) {
        std::cout << value << " ";
        i++;
        if (i % 6 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}