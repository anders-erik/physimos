#pragma once

#include "lib/arr.hh"



struct Mesh
{
    Arr<f3> verts;
    Arr<i3> faces;
    Arr<i2> edges;

    float color = 0.4f;
    bool wireframe = false;


    void init()
    {
    }


    static Mesh create_rectangle()
    {
        Mesh mesh;

        mesh.verts.set({}, 4);
        mesh.verts[0] = {-1.0f, -2.0f, 0.0f};
        mesh.verts[1] = {-1.0f,  2.0f, 0.0f};
        mesh.verts[2] = { 1.0f,  2.0f, 0.0f};
        mesh.verts[3] = { 1.0f, -2.0f, 0.0f};

        mesh.faces.set({}, 2);
        mesh.faces[0] = {0, 1, 2};
        mesh.faces[1] = {2, 3, 0};

        mesh.edges.set({}, 5);
        mesh.edges[0] = {0, 1};
        mesh.edges[1] = {1, 2};
        mesh.edges[2] = {2, 3};
        mesh.edges[3] = {3, 0};
        mesh.edges[4] = {1, 3};

        return mesh;
    }
};


