#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <string>
#include <vector>


/* 
    MTL
*/


float * obj_loadKdFromFile(std::string mtlPath);

void obj_loadMtlFromFile(std::string mtlPath);



/* 
    OBJ
*/

// One Vertex Coordinate
typedef struct VertexCoord {
    float x;
    float y;
    float z;
    float w;
} VertexCoord;

// One Vertex texture coordinate
typedef struct VertexTextCoord {
    float u;
    float v;
    float w;
} VertexTextCoord;

// One Vertex normal
typedef struct VertexNorm {
    float x;
    float y;
    float z;
} VertexNorm;



// One vertex - stored as integer indeces, identical to file
typedef struct ObjFaceVertexIndex {
    int v;
    int vt;
    int vn;
} ObjFaceVertexIndex;
// one 'f' row from obj-file
typedef struct ObjFaceIndex {
    ObjFaceVertexIndex vert1;
    ObjFaceVertexIndex vert2;
    ObjFaceVertexIndex vert3;
} ObjFaceIndex;



// Actual vertex values of face vertex
typedef struct ObjFaceVertex {
    VertexCoord v;
    VertexTextCoord vt;
    VertexNorm vn;
} ObjFaceVertex;
// Actual vertex values of whole face
typedef struct ObjFace{
    ObjFaceVertex vert1;
    ObjFaceVertex vert2;
    ObjFaceVertex vert3;
} ObjFace;



typedef struct ObjMesh {
    std::vector<VertexCoord> v;
    std::vector<VertexTextCoord> vt;
    std::vector<VertexNorm> vn;
    std::vector<ObjFaceIndex> fi;
    std::vector<ObjFace> f;
} ObjMesh;


std::vector<float> obj_getVertexBuffer_v_vt_vn();
void obj_loadFromFile(std::string objPath);


#endif