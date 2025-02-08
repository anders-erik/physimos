#ifndef TRANSFORM_HH
#define TRANSFORM_HH

#include "Types.hpp"
#include "math/vec3.hh"


class Transform {
    public:
        Transform() {};


        void Rotate(Vec3 rotationVector);
        void Translate(Vec3 moveVector);
        void SetPosition(Vec3 positionVector);


        ::pmath::Vec3 scale = ::pmath::Vec3( 1.0, 1.0, 1.0 );

        Vec3 position_0 = { 0.0f, 0.0f, 0.0f };;
        Vec3 position = { 0.0f, 0.0f, 0.0f };
        Vec3 translationPrevStep = { 0.0f, 0.0f, 0.0f };
        Vec3 velocity_0 = { 0.0f, 0.0f, 0.0f };;
        Vec3 velocityPrevStep = { 0.0f, 0.0f, 0.0f };
        Vec3 velocity = { 0.0f, 0.0f, 0.0f };

        Vec3 rotation_0 = { 0.0f, 0.0f, 0.0f };;
        Vec3 rotation = { 0.0f, 0.0f, 0.0f };
        Vec3 angularVelocity_0 = { 0.0f, 0.0f, 0.0f };;
        Vec3 angularVelocity = { 0.0f, 0.0f, 0.0f };

        
};



#endif