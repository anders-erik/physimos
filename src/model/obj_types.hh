#ifndef OBJ_TYPES__HH
#define OBJ_TYPES__HH

#include <string>
#include <vector>


namespace pmodel {


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
        float v; // opt ?
        float w; // opt ?
    } VertexTextCoord;

    // One Vertex normal
    typedef struct VertexNorm {
        float x;
        float y;
        float z;
    } VertexNorm;

    // One Vertex storing the index of each coordinate type
    typedef struct VertexIndeces {
        unsigned int vc_i;
        unsigned int vt_i;
        unsigned int vn_i;
    } VertexIndeces;

    // One Vertex storing the pointers of each coordinate type
    typedef struct VertexPointers {
        VertexCoord* vc_i;
        VertexTextCoord* vt_i;
        VertexNorm* vn_i;
    } VertexPointers;
    

    typedef struct TriangleFace {
        
    } TriangleFace;



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
    typedef struct ObjFace {
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


}


#endif