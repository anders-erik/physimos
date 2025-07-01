
#include "mesh.hh"

unsigned int Mesh::vert_size_bytes()
{
    return verts.size() * sizeof(Vertex);
}

void Mesh::
clear()
{
    verts.clear();
    faces.clear();
}


void Mesh::sheet()
{
    int WC = 10;            // width vertex count
    float square_width = 1; // actual width

    float step_size = square_width / (float) (WC - 1);

    #define I(x, y)     ((x) + (y) * WC) // Linear index in arrays

    verts.resize(WC*WC);
    for(int yi = 0; yi < WC; yi++)
    {
        for(int xi = 0; xi < WC; xi++)
        {
            verts[I(xi, yi)] =  {   step_size * xi,
                                    step_size * yi,
                                    0               };
        }
    }

    faces.clear();
    for(int yi = 0; yi < WC-1; yi++)
    {
        for(int xi = 0; xi < WC-1; xi++)
        {
            int i = I(xi, yi);

            faces.emplace_back(     i, 
                                    i + 1,  
                                    i + WC        );

            faces.emplace_back(     i + WC, 
                                    i + 1,  
                                    i + WC + 1    );
        }
    }
}


void Mesh::sheet(SheetContext context)
{

    int     WC = context.width_count;   // width vertex count
    float   W  = context.width;         // actual width

    float step_size = W / (float) (WC - 1);

    #define I(x, y)     ((x) + (y) * WC) // Linear index in arrays

    verts.resize(WC*WC);
    for(int yi = 0; yi < WC; yi++)
    {
        for(int xi = 0; xi < WC; xi++)
        {
            verts[I(xi, yi)] =  {   step_size * xi,
                                    step_size * yi,
                                    0               };
        }
    }

    faces.clear();
    for(int yi = 0; yi < WC-1; yi++)
    {
        for(int xi = 0; xi < WC-1; xi++)
        {
            int i = I(xi, yi);

            faces.emplace_back(     i, 
                                    i + 1,  
                                    i + WC        );

            faces.emplace_back(     i + WC, 
                                    i + 1,  
                                    i + WC + 1    );
        }
    }
}


void Mesh::
tube()
{

}


void Mesh::cube()
{
    verts.clear();
    faces.clear();

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;


    Vertex v00;
    v00.x = -cube_half;
    v00.y = -cube_half;
    v00.z = -cube_half;
    verts.push_back(v00);

    Vertex v01;
    v01.x =  cube_half;
    v01.y = -cube_half;
    v01.z = -cube_half;
    verts.push_back(v01);

    Vertex v02;
    v02.x =  cube_half;
    v02.y =  cube_half;
    v02.z = -cube_half;
    verts.push_back(v02);

    Vertex v03;
    v03.x = -cube_half;
    v03.y =  cube_half;
    v03.z = -cube_half;
    verts.push_back(v03);




    Vertex v10;
    v10.x = -cube_half;
    v10.y = -cube_half;
    v10.z =  cube_half;
    verts.push_back(v10);

    Vertex v11;
    v11.x =  cube_half;
    v11.y = -cube_half;
    v11.z =  cube_half;
    verts.push_back(v11);

    Vertex v12;
    v12.x =  cube_half;
    v12.y =  cube_half;
    v12.z =  cube_half;
    verts.push_back(v12);

    Vertex v13;
    v13.x = -cube_half;
    v13.y =  cube_half;
    v13.z =  cube_half;
    verts.push_back(v13);


    // Front
    faces.emplace_back(TriangleFaceIndeces{1, 5, 2});
    faces.emplace_back(TriangleFaceIndeces{2, 5, 6});

    // Right
    faces.emplace_back(TriangleFaceIndeces{2, 6, 3});
    faces.emplace_back(TriangleFaceIndeces{3, 6, 7});

    // Back
    faces.emplace_back(TriangleFaceIndeces{3, 7, 0});
    faces.emplace_back(TriangleFaceIndeces{0, 7, 4});

    // Left
    faces.emplace_back(TriangleFaceIndeces{0, 4, 1});
    faces.emplace_back(TriangleFaceIndeces{1, 4, 5});

    // Up
    faces.emplace_back(TriangleFaceIndeces{0, 1, 2});
    faces.emplace_back(TriangleFaceIndeces{2, 3, 0});

    // Down
    faces.emplace_back(TriangleFaceIndeces{4, 5, 6});
    faces.emplace_back(TriangleFaceIndeces{6, 7, 4});

}

void Mesh::quad()
{
    verts.clear();
    faces.clear();

    float quad_size = 1.0f;

    // Clock-wise 

    // 0
    verts.emplace_back( 0.0f,
                        0.0f,
                        0.0f );

    // 1
    verts.emplace_back( quad_size,
                        0.0f,
                        0.0f );

    // 2
    verts.emplace_back( quad_size,
                        quad_size,
                        0.0f );

    // 3
    verts.emplace_back( 0.0f,
                        quad_size,
                        0.0f );


    faces.emplace_back(0, 1, 2); // Lower right
    faces.emplace_back(0, 2, 3); // Upper left

}

void Mesh::center()
{
    f3 vert_sum;
    for(auto& vert : verts)
        vert_sum += vert;
    
    f3 vert_average = vert_sum / verts.size();

    for(auto& vert : verts)
        vert -= vert_average;
}


void Mesh::
scale(float factor)
{
    for(auto& vert : verts)
        vert *= factor;
}


void Mesh::
move(const f3& delta)
{
    for(auto& vert : verts)
        vert += delta;
}
