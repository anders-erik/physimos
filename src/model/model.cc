#include <cmath>

#include "opengl/texture.hh"

#include "model.hh"

namespace model {





void ModelColor::set_base_color(f3 color){

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


void model_center(MeshTexture& mesh){
    
    f3 center = {0.0f, 0.0f, 0.0f};

    for(VertexTexture& vertex : mesh.vertices){
        center.x += vertex.pos.x;
        center.y += vertex.pos.y;
        center.z += vertex.pos.z;
    }

    center.x /= (float) mesh.vertices.size();
    center.y /= (float) mesh.vertices.size();
    center.z /= (float) mesh.vertices.size();

    for(VertexTexture& vertex : mesh.vertices){
        vertex.pos.x -= center.x;
        vertex.pos.y -= center.y;
        vertex.pos.z -= center.z;
    }
}
void model_scale(MeshTexture& mesh, float scale){
    for(VertexTexture& vertex : mesh.vertices){
        vertex.pos.x *= scale;
        vertex.pos.y *= scale;
        vertex.pos.z *= scale;
    }
}
void model_translate(MeshTexture& mesh, f3 translation){

    for(VertexTexture& vertex : mesh.vertices){
        vertex.pos.x += translation.x;
        vertex.pos.y += translation.y;
        vertex.pos.z += translation.z;
    }

}
void model_rotate(MeshTexture& mesh, float angle_rad, f3 axis){
    m4f4 rot_matrix;

    if(axis.x == 1.0f){
        m4f4 tmp_mat = m4f4_create_rotation_x(1.57f);
        rot_matrix = tmp_mat;
    }
    else if(axis.y == 1.0f){
        m4f4 tmp_mat = m4f4_create_rotation_y(1.57f);
        rot_matrix = tmp_mat;
    }
    else if(axis.z == 1.0f){
        m4f4 tmp_mat = m4f4_create_rotation_z(1.57f);
        rot_matrix = tmp_mat;
    }

    for(VertexTexture& vertex : mesh.vertices){

        vertex.pos.matmul(rot_matrix);

    }

}




void model_generate_ground(MeshTexture& mesh){
    
    
    int square_width = 10;
    int verts_per_width = 100;

    float step_size = (float) square_width / (float) (verts_per_width - 1);

    // glm::vec2 tex_coords = gpu_get_color_coordinate(TextColor::Green);
    // glm::vec2 tex_coords = gpu_get_color_coordinate(TextColor::LightGray);
    // glm::vec2 tex_grass = gpu_get_color_coordinate(TextColor::Red);
    

    // VERTEX GENERATION
    for(int row = 0; row < verts_per_width; row++){
        for(int col = 0; col < verts_per_width; col++){

            VertexTexture& vertex = mesh.vertices.emplace_back();
            
            // POSITION

            float x = step_size * col;
            float y = step_size * row;
            // Will map to function later
            float z = 0;

            vertex.pos = {x, y, z};


            // NORMAL
            vertex.normal = {0.0f, 0.0f, 1.0f}; // Currently z = 0


            // Texture color
            // Maps square to single texture coordinate
            f2 tex_grass = opengl::texture_get_random_grass_color(row * verts_per_width + col);
            // glm::vec2 tex_grass = gpu_get_color_coordinate(TextColor::LightGray);
            vertex.tex = {
                tex_grass.x,
                tex_grass.y,
            }; 

            // model.vertices.push_back( {x, y, z} );
        
        }
    }


    // FACES
    // NOTE: loop width-1!
    for(int row = 0; row < verts_per_width-1; row++){
        for(int col = 0; col < verts_per_width-1; col++){

            // bottom left conrder of eqch square
            int idx_curr = row * verts_per_width + col;
            int idx_next_row = idx_curr + verts_per_width;

            mesh.faces.emplace_back(   idx_curr, 
                                        idx_curr + 1,  
                                        idx_next_row
            );

            mesh.faces.emplace_back(   idx_next_row, 
                                        idx_curr + 1,  
                                        idx_next_row + 1
            );
        }
    }
    

}



void model_generate_tube(MeshTexture& mesh, TubeContext tube_context){

    while(mesh.vertices.size() > 0)
        mesh.vertices.pop_back();
    while(mesh.faces.size() > 0)
        mesh.faces.pop_back();


    // glm::vec2 tex_coords = gpu_get_color_coordinate(TextColor::Blue);
    f2 tex_coords = opengl::texture_get_color_coordinate(opengl::TextColor::Black);


    // Check if vertices are generated correctly
    tube_context.vertex_count_target = tube_context.frame_count * tube_context.frame_point_count;

    // GENERATE VERTICES
    for( int frame_inx = 0; frame_inx < tube_context.frame_count; frame_inx++){
        for( int circ_inx = 0; circ_inx < tube_context.frame_point_count; circ_inx++){

            float angle = (6.28f / (float) tube_context.frame_point_count );
            angle *= (float) circ_inx;


            float x_pos = (float)frame_inx * tube_context.frame_gap;
            float y_pos = std::sin(angle) * tube_context.radius;
            float z_pos = std::cos(angle) * tube_context.radius;

            VertexTexture vertex;

            vertex.pos = { x_pos, y_pos, z_pos};


            // Vertex normals
            // vertex.normal = { 1.0f, 1.0f, 1.0f };
            vertex.normal = { 0.0f, y_pos, z_pos };


            // Texture coordinates
            vertex.tex = {
                tex_coords.x,
                tex_coords.y,
            };


            mesh.vertices.push_back(vertex);
        }
    }


    // POPULATE INDECES 

    // connect triangles from the current frame to the next one. Loop until frame_count-1!
    for( int frame_inx = 0; frame_inx < tube_context.frame_count - 1; frame_inx++){
        
        // circle index will be offset by global index of first vertex in frame
        int first_vert_index = frame_inx * tube_context.frame_point_count;

        // Current frame start index
        int m_0 = first_vert_index;
        // Next frame start index
        int n_0 = first_vert_index + tube_context.frame_point_count;

        // Frame loop points looping
        for( int loop_i = 0; loop_i < tube_context.frame_point_count; loop_i++){
            
            // NOTE: Index wraps on three vertices during the last quad in frame section
            bool index_wrap = loop_i == tube_context.frame_point_count - 1;

            // Triangular Face 1
            TriangleFaceIndeces face_1;
            face_1.v0 = m_0 + loop_i;
            face_1.v1 = n_0 + loop_i;
            face_1.v2 = index_wrap ? m_0 : m_0 + loop_i + 1 ;

            // Triangular Face 2
            TriangleFaceIndeces face_2;
            face_2.v0 = index_wrap ? m_0 : m_0 + loop_i + 1;
            face_2.v1 = n_0 + loop_i;
            face_2.v2 = index_wrap ? n_0 : n_0 + loop_i + 1;
            
            mesh.faces.push_back(face_1);
            mesh.faces.push_back(face_2);

        }
    }

}




}