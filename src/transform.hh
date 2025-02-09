#ifndef TRANSFORM_HH
#define TRANSFORM_HH

#include "Types.hpp"
#include "math/vec3.hh"


class Transform {
    public:

        ::pmath::Vec3 scale = ::pmath::Vec3( 1.0, 1.0, 1.0 );

        ::pmath::Vec3 position = { 0.0f, 0.0f, 0.0f };
        ::pmath::Vec3 velocity = { 0.0f, 0.0f, 0.0f };

        ::pmath::Vec3 rotation = { 0.0f, 0.0f, 0.0f };
        ::pmath::Vec3 angularVelocity = { 0.0f, 0.0f, 0.0f };
        

        Transform() {};
};



#endif