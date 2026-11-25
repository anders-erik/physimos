#pragma once

#include "lib/arr.hh"

/* Mesh vertex consiting of 3 vertex coordinates and one color value. */
struct V3C1
{
    f3 v; // vertex
    float c; // color
};

struct Mesh
{
    Arr<f3> verts;
    Arr<i3> faces;

    float color = 0.4f;


    void init()
    {
        verts.set({}, 6);
        verts[0] = {-1.0f,  2.0f, 0.0f};
        verts[1] = {-1.0f, -2.0f, 0.0f};
        verts[2] = { 1.0f, -2.0f, 0.0f};
        verts[3] = { 1.0f,  2.0f, 0.0f};
        // verts[4] = { 1.0f, -2.0f, 0.0f};
        // verts[5] = {-1.0f,  2.0f, 0.0f};

        faces.set({}, 2);
        faces[0] = {0, 1, 2};
        faces[0] = {1, 2, 3};
    }
};

