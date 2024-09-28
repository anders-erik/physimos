// Holds / loads models, verticies, textures, etc.

#ifndef MODEL_HPP
#define MODEL_HPP


#include <vector>
#include <string>

#include "object_types.hh"

namespace objects {


class Model {
    public:
        Model() {};
        Model(std::string _modelname);

        MODELNAME modelname;

        void setVaoVbo_obj();
        void loadObjModel(std::string objModelName);
        void useTexture();


        std::vector<float> vertices;
        int vertexCount;

        unsigned int vao;
        unsigned int vbo;

        // Texture
        // int hasTexture = 0;
        unsigned int glTexture = 0;
        // unsigned int* textureBuffer;
        // int hasMtlKd = 0;
        // float Kd[3] = { 0.0f, 0.0f, 0.0f };

};


}


#endif