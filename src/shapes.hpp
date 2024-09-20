#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>


const std::vector<float> trianglePoints = {
    0.0f, 1.0f, -1.0f,
    0.0f, -1.0f, -1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, -1.0f,
    0.0f, -1.0f, -1.0f,
    0.0f, 0.0f, 1.0f,
};


const std::vector<float> simulatorPoints = {
    // Front face
    -1.0f, 1.0f, -1.0f, // bottom left
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f, // bottom right
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, // top right
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // top left
    -1.0f, 1.0f, -1.0f,
    // Back face
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    // link above four corners
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
};



#endif