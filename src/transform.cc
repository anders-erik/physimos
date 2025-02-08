
#include "transform.hh"




void Transform::Rotate(Vec3 rotationVector) {
    this->rotation.x += rotationVector.x;
    this->rotation.y += rotationVector.y;
    this->rotation.z += rotationVector.z;
}

void Transform::Translate(Vec3 moveVector) {
    this->position.x += moveVector.x;
    this->position.y += moveVector.y;
    this->position.z += moveVector.z;
}
void Transform::SetPosition(Vec3 positionVector) {
    this->position.x = positionVector.x;
    this->position.y = positionVector.y;
    this->position.z = positionVector.z;
}


