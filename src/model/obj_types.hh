#ifndef OBJ_TYPES_HH
#define OBJ_TYPES_HH

#include <string>


namespace pmodel {
namespace pobj {

    extern float emptyTextureCoord;

    // Mtl colors, usually represented by 'Kx'
    typedef struct MtlColor {
        float R = 0.0f;
        float G = 0.0f;
        float B = 0.0f;
    } MtlColor;

    typedef struct Mtl {

        std::string name;
        std::string map_Kd; // File name of Diffuse texture

        MtlColor Ka;       // Ambient Color
        MtlColor Ks;       // Specular Color
        MtlColor Kd;       // Diffuse Color
        MtlColor Ke;       // Emissive coeficient

        float Ns;   // Specular exponent
        float Ni;   // optical density / index of refraction

        float d;    // Dissolve / transparancy : d = 1 - Tr

        int illum; // illumination: 1-10

    } Mtl;


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
        Mtl* mtl;   // The active material when TriangleFaceI was parsed from obj file
    } TriangleFaceI;


    // For the Physimos mesh type.
    // One Vertex storing the pointers of each coordinate type
    // typedef struct VertexPointers {
    //     VertexCoord*        vc_i;
    //     VertexTextureCoord* vt_i;
    //     VertexNormal*       vn_i;
    // } VertexPointers;

}
}


#endif