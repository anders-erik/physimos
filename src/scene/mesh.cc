
#include <cmath>

#include "math/const.hh"

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



void Mesh::cube()
{
    verts.clear();
    faces.clear();

    float cube_size = 1.0f;
    float cube_half = cube_size / 2.0f;

    // -Z
    verts.emplace_back( -cube_half, 
                        -cube_half,
                        -cube_half  );
    
    verts.emplace_back(  cube_half, 
                        -cube_half,
                        -cube_half  );

    verts.emplace_back(  cube_half, 
                         cube_half,
                        -cube_half  );

    verts.emplace_back( -cube_half, 
                         cube_half,
                        -cube_half  );

    // +Z
    verts.emplace_back( -cube_half, 
                        -cube_half,
                         cube_half  );
    
    verts.emplace_back(  cube_half, 
                        -cube_half,
                         cube_half  );

    verts.emplace_back(  cube_half, 
                         cube_half,
                         cube_half  );

    verts.emplace_back( -cube_half, 
                         cube_half,
                         cube_half  );


    // Front
    faces.emplace_back( 1, 5, 2 );
    faces.emplace_back( 2, 5, 6 );
    // Right
    faces.emplace_back( 2, 6, 3 );
    faces.emplace_back( 3, 6, 7 );
    // Back
    faces.emplace_back( 3, 7, 0 );
    faces.emplace_back( 0, 7, 4 );
    // Left
    faces.emplace_back( 0, 4, 1 );
    faces.emplace_back( 1, 4, 5 );
    // Up
    faces.emplace_back( 0, 1, 2 );
    faces.emplace_back( 2, 3, 0 );
    // Down
    faces.emplace_back( 4, 5, 6 );
    faces.emplace_back( 6, 7, 4 );
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
scale_z(float factor)
{
    for(auto& vert : verts)
        vert.z *= factor;
}

void Mesh::
move_z(float delta)
{
    for(auto& vert : verts)
        vert.z += delta;
}


void Mesh::
move(const f3& delta)
{
    for(auto& vert : verts)
        vert += delta;
}




void Mesh::
tube(TubeContext t_context)
{
    verts.clear();
    faces.clear();
    normals.clear();

    int FC  = t_context.frame_count;        // Frame count
    int CPC = t_context.circle_point_count; // Circle point count

    size_t OTVC = FC * CPC;     // Open tube vertex count
    size_t CTVC = FC * CPC + 2; // Closed tube vertex count

    size_t index_bottom    = CTVC - 2;
    size_t index_top       = CTVC - 1;

    // closed tubes only for now
    verts.reserve(CTVC);
    normals.reserve(CTVC);


    float frame_gap     = 1.0f / ( (float) (FC - 1));
    float angle_delta   = PI2f / ( (float) CPC );

    // Index : Mesh Verts
    #define IMV(i_cp, i_f)     ((i_cp) + (i_f) * CPC)

    int i_fr;   // index: frame
    int i_cp;   // index : circle point

    // Open Tube Verts
    for( i_fr = 0; i_fr < FC; i_fr++)
    {
        for( i_cp = 0; i_cp < CPC; i_cp++)
        {
            float angle = angle_delta * (float) i_cp;

            verts.emplace_back( cos(angle), 
                                sin(angle), 
                                (float) i_fr * frame_gap  );
            normals.emplace_back(   cos(angle), 
                                    sin(angle), 
                                    0.0f        );
        }
    }

    // Open Tube Faces
    for( i_fr = 0; i_fr < FC - 1; i_fr++) // until N-1 as faces are created using frame n+1
    {
        for( i_cp = 0; i_cp < CPC; i_cp++)
        {
            int i_cp_p1_mod = (i_cp+1) % CPC; // if i_cp + 1 == CPC, then vert is equal to first vert

            faces.emplace_back( IMV( i_cp        , i_fr   ), 
                                IMV( i_cp_p1_mod , i_fr   ), 
                                IMV( i_cp        , i_fr+1 )    );

            faces.emplace_back( IMV( i_cp_p1_mod , i_fr   ), 
                                IMV( i_cp_p1_mod , i_fr+1 ), 
                                IMV( i_cp        , i_fr+1 )    );
        }
    }

    // bottom/top verts
    verts.emplace_back(0.0f, 0.0f, 0.0f);
    verts.emplace_back(0.0f, 0.0f, 1.0f);
    normals.emplace_back(0.0f, 0.0f, -1.0f);
    normals.emplace_back(0.0f, 0.0f,  1.0f);

    // close bottom
    for(i_cp = 0; i_cp < CPC; i_cp++)
    {
        int i_cp_p1_mod = (i_cp+1) % CPC; // if i_cp + 1 == CPC, then vert is equal to first vert
        faces.emplace_back( index_bottom,
                            IMV( i_cp        , 0 ), 
                            IMV( i_cp_p1_mod , 0 )    );
    }

    // close top
    for(i_cp = 0; i_cp < CPC; i_cp++)
    {
        int i_cp_p1_mod = (i_cp+1) % CPC; // if i_cp + 1 == CPC, then vert is equal to first vert
        faces.emplace_back( index_top,
                            IMV( i_cp        , FC-1 ), 
                            IMV( i_cp_p1_mod , FC-1 )    );
    }


}



void Mesh::tube_poly_z(TubeContext t_context, Polynomial<float> polynomial)
{
    int FC  = t_context.frame_count;        // Frame count
    int CPC = t_context.circle_point_count; // Circle point count

    // Index : Mesh Verts
    #define IMV(i_cp, i_f)     ((i_cp) + (i_f) * CPC)

    float frame_gap     = 1.0f / ( (float) IMV(0, 1) - IMV(0, 0));
    float angle_delta   = PI2f / ( (float) CPC );

    float dz = 2.0f * frame_gap;

    int i_fr;   // index: frame
    int i_cp;   // index : circle point

    // verts
    for(i_fr=0; i_fr<FC; i_fr++)
    {
        float radius = polynomial(verts[IMV(0, i_fr)].z);

        for(i_cp=0; i_cp<CPC; i_cp++)
        {
            f3& vert = verts[IMV(i_cp, i_fr)];

            float angle = angle_delta * i_cp;

            vert.x = radius * cos(angle);
            vert.y = radius * sin(angle);
        }
    }

    // normals
    for(i_fr=0; i_fr<FC; i_fr++)
    {
        float radius = polynomial(verts[IMV(0, i_fr)].z);

        float r_p1 = (i_fr == FC) ? 0.0f : polynomial(verts[IMV(0, i_fr+1)].z);
        float r_m1 = (i_fr == 0 ) ? 0.0f : polynomial(verts[IMV(0, i_fr-1)].z);
        float dr = r_p1 - r_m1;

        float dr_dz  = dr / dz;
        float dz_dr  = -1.0f / dr_dz; // Normal to dr_dz
        // float z = radius * dz_dr; // z component of normal
        float z = radius * -dr_dz; // Works

        for(i_cp=0; i_cp<CPC; i_cp++)
        {
            f3& vert =   verts[IMV(i_cp, i_fr)];
            f3& norm = normals[IMV(i_cp, i_fr)];

            norm = {vert.x, vert.y, z};
            norm = norm.unit();
            norm.norm();
        }
    }


    // Center top/bottom verts
    verts[verts.size()-2].x = 0.0f;
    verts[verts.size()-2].y = 0.0f;
    verts[verts.size()-1].x = 0.0f;
    verts[verts.size()-1].y = 0.0f;
    normals[normals.size()-2].x =  0.0f;
    normals[normals.size()-2].y =  0.0f;
    normals[normals.size()-2].z = -1.0f;
    normals[normals.size()-1].x =  0.0f;
    normals[normals.size()-1].y =  0.0f;
    normals[normals.size()-1].z =  1.0f;
}
