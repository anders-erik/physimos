
#include "Model.hpp"

#include <string>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "obj_loader.hpp"
#include "object_types.hh"
#include "loaded_resources.hh"


// EXTERNAL OBJ  PROPERTIES
extern float Kd[3];
extern bool hasTextureMap;

extern std::vector<unsigned char> obj_textureDataBuffer;
extern unsigned int obj_imgWidth;
extern unsigned int obj_imgHeight;

namespace objects {

    // keeps track of loaded models
    // std::map<objects::MODELNAME, bool> modelLoadedMap;


    Model::Model(std::string _modelname) {

        // TODO : Implement more than just obj models!

        loadObjModel(_modelname);

    };


    void Model::useTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glTexture);
    }

    void Model::setVaoVbo_obj() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        // REPLACE VERTEX NORMALS WITH COLORS!
        for (unsigned int i = 0; i < vertices.size(); i += 8) {
            // vertices[i + 5] = 0.0f;
            // vertices[i + 6] = 0.0f;
            // vertices[i + 7] = 0.0f;

            // vertices[i + 5] = Kd[0];
            // vertices[i + 6] = Kd[1];
            // vertices[i + 7] = Kd[2];

            // model.vertices[i + 5] = model.Kd[0];
            // model.vertices[i + 6] = model.Kd[1];
            // model.vertices[i + 7] = model.Kd[2];
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        // printf("triangel : %lu\n", sizeof(triangle));
        // printf("data()   : %lu\n", sizeof(worldCube1.vertices.data()); // I guess this returns the size of the pointer to the underlaying data/array on the heap?
        // printf("data()   : %lu\n", sizeof(worldCube1.vertices[0])*worldCube1.vertices.size());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }


    void Model::loadObjModel(std::string objModelName) {

        std::cout << "Loading obj model: " << objModelName << ". ";

        obj_loadFromFile(objModelName);


        // OBJ
        std::vector<float> _vertBuffer = obj_getVertexBuffer_v_vt_vn();
        // vertices = obj_getVertexBuffer_v_vt_vn();
        for (float vertElement : _vertBuffer)
            vertices.push_back(vertElement);
        // std::cout << "worldCube4_obj.vertices.size() = " << worldCube4_obj.vertices.size() << std::endl;
        vertexCount = vertices.size() / 8;

        std::cout << " OK. [" << vertices.size() << " values]" << std::endl;

        // MTL
        // obj_loadMtlFromFile("./resources/models/blend-cube-no-texture.mtl");
        // obj_loadMtlFromFile("./resources/models/blend-cube-texture-1/blend-cube-texture-1.mtl");
        obj_loadMtlFromFile(objModelName);
        // float* Kd_temp = obj_loadKdFromFile("./resources/models/blend-cube-no-texture.mtl");

        // IF THERE IS NO TEXTURE, A SIMNGLE PIXEL TEXTURE IS CREATED USING KD-VALUES IN MTL FILE
        if (!hasTextureMap) {
            // std::cout << "NO TEXTURE MAP" << std::endl;

            // Kd[0] = Kd_temp[0];
            // Kd[1] = Kd_temp[1];
            // Kd[2] = Kd_temp[2];
            // this->Kd[0] = Kd[0];
            // this->Kd[1] = Kd[1];
            // this->Kd[2] = Kd[2];

            // LOAD TEXTURE
            glGenTextures(1, &glTexture);
            glBindTexture(GL_TEXTURE_2D, glTexture);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // bmp_loader_loadBMPFile("media/mountain.bmp");
            // bmp_loader_loadBMPFile("resources/models/blend-cube-texture-1.bmp");

            // blend-cube-texture-1
            // bmp_loader.prettyPrint();

            // float Kd_color = 0.5;

            unsigned char Kd_x = (unsigned char)(Kd[0] * 255.0);
            unsigned char Kd_y = (unsigned char)(Kd[1] * 255.0);
            unsigned char Kd_z = (unsigned char)(Kd[2] * 255.0);
            // std::cout << "Kd_x = " << Kd_x << std::endl;

            // const unsigned char kd_Data[3] = { Kd[0], Kd[1], Kd[2] };
            const unsigned char kd_Data[3] = { Kd_x, Kd_y, Kd_z };
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &kd_Data);
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &kd_Data);
            glGenerateMipmap(GL_TEXTURE_2D);



        }
        else {
            // std::cout << "HAS TEXTURE MAP" << std::endl;

            // LOAD TEXTURE
            glGenTextures(1, &glTexture);
            glBindTexture(GL_TEXTURE_2D, glTexture);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // bmp_loader_loadBMPFile("media/mountain.bmp");
            // bmp_loader_loadBMPFile("resources/models/blend-cube-texture-1.bmp");

            // blend-cube-texture-1
            // bmp_loader.prettyPrint();

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, obj_imgWidth, obj_imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, obj_textureDataBuffer.data());
            glGenerateMipmap(GL_TEXTURE_2D);

        }

        // if mtl doesn't have texture file, generate color texture from Kd

        // this->Kd[0] = Kd[0];
        // this->Kd[1] = Kd[1];
        // this->Kd[2] = Kd[2];

        // std::cout << "Loading obj model: " << objModelName << ". " << std::endl;


        // obj_loadMtlFromFile("./resources/models/blend-cube-texture-1/blend-cube-texture-1.mtl");
        // if (!hasTextureMap) {
        //     std::cout << "NO TEXTURE MAP" << std::endl;
        // }
        // else {
        //     std::cout << "HAS TEXTURE MAP" << std::endl;
        // }

    }


}
