#ifndef TRANSFORM_HH
#define TRANSFORM_HH

#include "Types.hpp"


class Transform {
    public:
        Transform() {};


        void Rotate(Vec3 rotationVector);
        void Translate(Vec3 moveVector);
        void SetPosition(Vec3 positionVector);
        void SetScale(Vec3 scaleVector);


        Vec3 scale = { 1.0f, 1.0f, 1.0f };

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