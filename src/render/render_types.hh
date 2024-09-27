#ifndef RENDER_TYPES_HH
#define RENDER_TYPES_HH

#include "object_types.hh"

namespace render {




typedef enum MODEL_SOURCE {
    code,
    obj,
    pos,
} MODEL;

typedef enum VDF {
    p3c3,
    p3n3t2,
} VDF;


typedef enum SHADER {
    world_obj_shader,
} SHADER;



typedef struct Rendpipe {
    objects::MODELNAME modelname; // we need to make sure this is compatible with the render-specific fields of the struct
    MODEL_SOURCE modelSource;
    VDF vfd;
    SHADER shader;
} Rendpipe;

typedef enum RENDPIPE {
    worldObject_noLight = 100, // implies : p3n3t2 + world_obj_shader + obj
} RENDPIPE;


}



#endif
