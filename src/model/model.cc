#include <cmath>


#include "model.hh"

namespace xpeditor {




void Model::set_transform_matrix(){
    m4f4 identity;

    transform.matrix = identity;

    transform.matrix.translate(transform.pos);
    
    // Intrinsic rotations work??
    // transform.matrix.rotate_x(transform.rot.x);
    // transform.matrix.rotate_y(transform.rot.y);
    // transform.matrix.rotate_z(transform.rot.z);

    // mat_mul(transform.matrix, );

}
void Model::set_transform_matrix_anim(){
    m4f4 identity;

    

    // transform.matrix = identity;

    // transform.matrix.rotate_z(transform.rot.z);
    // transform.matrix.rotate_y(transform.rot.y);
    // transform.matrix.rotate_x(transform.rot.x);

    // // transform.matrix.translate(transform.pos);

    // transform.matrix = transform.matrix;

}

void Model::set_base_color(f3 color){

    for(VertexColor& vert : mesh.vertices){
        vert.color.x = color.x;
        vert.color.y = color.y;
        vert.color.z = color.z;
    }
}

void model_add_facelet(MeshColor& mesh, f3 color, Axis axis){

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;

    float facelet_offset = cube_half + 0.001;
    float facelet_width = cube_half - 0.05;

    VertexColor v0;
    VertexColor v1;
    VertexColor v2;
    VertexColor v3;

    if(axis == Axis::x || axis == Axis::nx){

        v0.pos = f3( 0.0f, -facelet_width, -facelet_width);
        v1.pos = f3( 0.0f,  facelet_width, -facelet_width);
        v2.pos = f3( 0.0f,  facelet_width,  facelet_width);
        v3.pos = f3( 0.0f, -facelet_width,  facelet_width);

        if(axis == Axis::x){
            v0.pos.x += facelet_offset;
            v1.pos.x += facelet_offset;
            v2.pos.x += facelet_offset;
            v3.pos.x += facelet_offset;
            v0.normal.x = 1.0f;
            v1.normal.x = 1.0f;
            v2.normal.x = 1.0f;
            v3.normal.x = 1.0f;
        }
        else{
            v0.pos.x -= facelet_offset;
            v1.pos.x -= facelet_offset;
            v2.pos.x -= facelet_offset;
            v3.pos.x -= facelet_offset;
            v0.normal.x = -1.0f;
            v1.normal.x = -1.0f;
            v2.normal.x = -1.0f;
            v3.normal.x = -1.0f;
        }
    }
    else if(axis == Axis::y || axis == Axis::ny){

        v0.pos = f3( -facelet_width, 0.0f, -facelet_width);
        v1.pos = f3(  facelet_width, 0.0f, -facelet_width);
        v2.pos = f3(  facelet_width, 0.0f,  facelet_width);
        v3.pos = f3( -facelet_width, 0.0f,  facelet_width);

        if(axis == Axis::y){
            v0.pos.y += facelet_offset;
            v1.pos.y += facelet_offset;
            v2.pos.y += facelet_offset;
            v3.pos.y += facelet_offset;
            v0.normal.y = 1.0f;
            v1.normal.y = 1.0f;
            v2.normal.y = 1.0f;
            v3.normal.y = 1.0f;
        }
        else{
            v0.pos.y -= facelet_offset;
            v1.pos.y -= facelet_offset;
            v2.pos.y -= facelet_offset;
            v3.pos.y -= facelet_offset;
            v0.normal.y = -1.0f;
            v1.normal.y = -1.0f;
            v2.normal.y = -1.0f;
            v3.normal.y = -1.0f;
        }
    }
    else if(axis == Axis::z || axis == Axis::nz){

        v0.pos = f3( -facelet_width, -facelet_width, 0.0f);
        v1.pos = f3(  facelet_width, -facelet_width, 0.0f);
        v2.pos = f3(  facelet_width,  facelet_width, 0.0f);
        v3.pos = f3( -facelet_width,  facelet_width, 0.0f);

        if(axis == Axis::z){
            v0.pos.z += facelet_offset;
            v1.pos.z += facelet_offset;
            v2.pos.z += facelet_offset;
            v3.pos.z += facelet_offset;
            v0.normal.z = 1.0f;
            v1.normal.z = 1.0f;
            v2.normal.z = 1.0f;
            v3.normal.z = 1.0f;
        }
        else{
            v0.pos.z -= facelet_offset;
            v1.pos.z -= facelet_offset;
            v2.pos.z -= facelet_offset;
            v3.pos.z -= facelet_offset;
            v0.normal.z = -1.0f;
            v1.normal.z = -1.0f;
            v2.normal.z = -1.0f;
            v3.normal.z = -1.0f;
        }
    }



    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;

    mesh.vertices.push_back(v0);
    mesh.vertices.push_back(v1);
    mesh.vertices.push_back(v2);
    mesh.vertices.push_back(v3);
    
    int v0i = mesh.vertices.size()-4;
    int v1i = mesh.vertices.size()-3;
    int v2i = mesh.vertices.size()-2;
    int v3i = mesh.vertices.size()-1;

    mesh.faces.emplace_back(TriangleFaceIndeces{v0i, v1i, v2i});
    mesh.faces.emplace_back(TriangleFaceIndeces{v2i, v3i, v0i});

}

void model_add_cube_mesh(MeshColor& mesh){

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;


    // for(int crossec_i = 0; crossec_i < 2; crossec_i++){
    //     for(int vert_i = 0; vert_i < 4; vert_i++){


        
    //     }
    // }


    VertexColor v00;
    v00.pos.x = -cube_half;
    v00.pos.y = -cube_half;
    v00.pos.z = -cube_half;
    // v00.color.x = 0.0f;
    // v00.color.y = 0.0f;
    // v00.color.z = 0.0f;
    mesh.vertices.push_back(v00);

    VertexColor v01;
    v01.pos.x =  cube_half;
    v01.pos.y = -cube_half;
    v01.pos.z = -cube_half;
    mesh.vertices.push_back(v01);

    VertexColor v02;
    v02.pos.x =  cube_half;
    v02.pos.y =  cube_half;
    v02.pos.z = -cube_half;
    mesh.vertices.push_back(v02);

    VertexColor v03;
    v03.pos.x = -cube_half;
    v03.pos.y =  cube_half;
    v03.pos.z = -cube_half;
    mesh.vertices.push_back(v03);




    VertexColor v10;
    v10.pos.x = -cube_half;
    v10.pos.y = -cube_half;
    v10.pos.z =  cube_half;
    mesh.vertices.push_back(v10);

    VertexColor v11;
    v11.pos.x =  cube_half;
    v11.pos.y = -cube_half;
    v11.pos.z =  cube_half;
    mesh.vertices.push_back(v11);

    VertexColor v12;
    v12.pos.x =  cube_half;
    v12.pos.y =  cube_half;
    v12.pos.z =  cube_half;
    mesh.vertices.push_back(v12);

    VertexColor v13;
    v13.pos.x = -cube_half;
    v13.pos.y =  cube_half;
    v13.pos.z =  cube_half;
    mesh.vertices.push_back(v13);

    // Color & Normals
    for(VertexColor& vert : mesh.vertices){
        vert.color.x = 0.3f;
        vert.color.y = 0.3f;
        vert.color.z = 0.3f;

        // extend vert position from center
        float len_2 = vert.pos.x * vert.pos.x + vert.pos.y * vert.pos.y + vert.pos.z * vert.pos.z;
        float len = sqrtf(len_2);
        vert.normal.x = vert.pos.x / len;
        vert.normal.y = vert.pos.y / len;
        vert.normal.z = vert.pos.z / len;
    }

    // Front
    mesh.faces.emplace_back(TriangleFaceIndeces{1, 5, 2});
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 5, 6});

    // Right
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 6, 3});
    mesh.faces.emplace_back(TriangleFaceIndeces{3, 6, 7});

    // Back
    mesh.faces.emplace_back(TriangleFaceIndeces{3, 7, 0});
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 7, 4});

    // Left
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 4, 1});
    mesh.faces.emplace_back(TriangleFaceIndeces{1, 4, 5});

    // Up
    mesh.faces.emplace_back(TriangleFaceIndeces{0, 1, 2});
    mesh.faces.emplace_back(TriangleFaceIndeces{2, 3, 0});

    // Down
    mesh.faces.emplace_back(TriangleFaceIndeces{4, 5, 6});
    mesh.faces.emplace_back(TriangleFaceIndeces{6, 7, 4});

    

}



}