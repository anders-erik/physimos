// Holds / loads models, verticies, textures, etc.

#ifndef MODEL_HPP
#define MODEL_HPP


#include <vector>
#include <string>

class Model {
    public:
        Model() {};

        void loadObjModel(std::string objModelName);

        std::vector<float> vertices;
        int vertexCount;

        // Texture
        // int hasTexture = 0;
        unsigned int glTexture = 0;
        // unsigned int* textureBuffer;
        // int hasMtlKd = 0;
        // float Kd[3] = { 0.0f, 0.0f, 0.0f };

};


#endif