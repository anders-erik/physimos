// Holds / loads models, verticies, textures, etc.

#ifndef MODEL_HPP
#define MODEL_HPP


#include <vector>
#include <string>

// #include "object_types.hh"
#include "resources.hh"

namespace objects {


class Model {
    public:
        Model() {};
        Model(std::string _modelname);

        // MODELNAME modelname;

        // OBJ
        void loadObjModel(std::string objModelName);
        void setVaoVbo_obj();
        void useTexture();


        // PSO
        void loadPsoModel(std::string path);
        void setVaoVbo_p3c3();
        res::ModelVertStucture loadedVertStructure;
                

        std::vector<float> vertices;
        int vertexCount;
        unsigned int vao;
        unsigned int vbo;

        // Additional derived models:
        // 1. wireframe/bounding box
        //      - wireVao, wireVbo
        //      - for viewport clicks + collision!
        

        // Texture
        // int hasTexture = 0;
        unsigned int glTexture = 0;
        // unsigned int* textureBuffer;
        // int hasMtlKd = 0;
        // float Kd[3] = { 0.0f, 0.0f, 0.0f };

};


}


#endif