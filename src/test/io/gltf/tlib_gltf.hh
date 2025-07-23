#pragma once

#include "test/test.hh"
#include "test/io/gltf/tmod_all.hh"


TCModule gltf_all = {
    "gltf_all",{
        {"camera",      camera},
    }
};

TCLibrary tclib_gltf = {
    "gltf",
    {
        gltf_all,
    },
};


