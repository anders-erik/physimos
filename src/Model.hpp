// Holds / loads models, verticies, textures, etc.

#ifndef MODEL_HPP
#define MODEL_HPP


#include <vector>
#include <string>

// #include "object_types.hh"
#include "res/resources.hh"

namespace objects {

void generateRandomTexture();

typedef struct PVert{
    float x;
    float y;
    float z;
} PVert;

typedef struct PVertPair {
    PVert vert1;
    PVert vert2;
} PVertPair;

bool pVertPairsAreEquivelent(PVertPair& pair1, PVertPair& pair2);


class Model {
    public:
        Model() {};
        Model(std::string _modelname);

        // MODELNAME modelname;

        // OBJ || PSO, currently
        std::string modelname;
        res::ModelFileType modelFileType;

        // OBJ
        void loadObjModel(std::string objModelName);
        void setVaoVbo_obj();

        // PSO
        void loadPsoModel(std::string path);
        void setVaoVbo_p3c3();
        void setVaoVbo_p3c3t2();
        void setVaoVbo_p3();
        res::ModelVertStucture loadedVertStructure;

        // TEXTURE   
        unsigned int glTexture = 0;
        void useTexture();
        void createGLTexture(unsigned int imgWith, unsigned int imgHeight, unsigned char* imgData);


        // MODEL DATA
        std::vector<float> vertices;
        int vertexCount = 0;
        unsigned int vao = 0;
        unsigned int vbo = 0;

        // WIREFRAME
        void generateWireframeFromModel();
        std::vector<float> wireData;
        int wireVertexCount = 0;
        unsigned int vaoWire = 0;
        unsigned int vboWire = 0;

        // Additional derived models:
        // 1. wireframe/bounding box
        //      - wireVao, wireVbo
        //      - for viewport clicks + collision!
        

        // Texture
        // int hasTexture = 0;
        // unsigned int* textureBuffer;
        // int hasMtlKd = 0;
        // float Kd[3] = { 0.0f, 0.0f, 0.0f };

};


}


#endif