#ifndef OBJ_TYPES__HH
#define OBJ_TYPES__HH

#include <string>
#include <vector>


namespace pmodel {

    extern float emptyTextureCoord;


    // One Vertex Coordinate
    typedef struct VertexCoord {
        float x;
        float y;
        float z;
        float w;
    } VertexCoord;

    // One Vertex texture coordinate
    typedef struct VertexTextureCoord {
        float u;
        float v; // opt ?
        float w; // opt ?
    } VertexTextureCoord;

    // One Vertex normal
    typedef struct VertexNormal {
        float x;
        float y;
        float z;
    } VertexNormal;

    // One Vertex storing the obj index of each coordinate type
    typedef struct VertexI {
        unsigned int vc_i;
        unsigned int vt_i;
        unsigned int vn_i;
    } VertexI;

    
    // A face represented by three vertices, each vertes is represented by their respective obj-file index.
    typedef struct TriangleFaceI {
        VertexI v1;
        VertexI v2;
        VertexI v3;
    } TriangleFaceI;

    
    // One Vertex storing the pointers of each coordinate type
    // Physimos mesh type
    typedef struct VertexPointers {
        VertexCoord*        vc_i;
        VertexTextureCoord* vt_i;
        VertexNormal*       vn_i;
    } VertexPointers;



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
        VertexTextureCoord vt;
        VertexNormal vn;
    } ObjFaceVertex;
    // Actual vertex values of whole face
    typedef struct ObjFace {
        ObjFaceVertex vert1;
        ObjFaceVertex vert2;
        ObjFaceVertex vert3;
    } ObjFace;



    typedef struct ObjMesh {
        std::vector<VertexCoord> v;
        std::vector<VertexTextureCoord> vt;
        std::vector<VertexNormal> vn;
        std::vector<ObjFaceIndex> fi;
        std::vector<ObjFace> f;
    } ObjMesh;


}


#endif