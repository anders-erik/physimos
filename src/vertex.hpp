#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <cmath>

// #include "Camera.hpp"
#include "Simulation.hpp"


// Simulation Object methods
void RotationSimObject(struct SimObject * simObject, Point3 rotationVector);
void SetRotationSimObject(struct SimObject* simObject, Point3 rotationVector);
void SetScaleSimObject(struct SimObject * simObject, Point3 scaleVector);
void MoveSimObject(struct SimObject * simObject, Point3 moveVector);
void SetPositionSimObject(struct SimObject* simObject, Point3 moveVector);
void SetSimObjectTranform(struct SimObject * simObject);




#endif