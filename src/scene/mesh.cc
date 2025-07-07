
#include <cmath>

#include "math/const.hh"

#include "opengl/color.hh"

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

void Mesh::merge(const Mesh& mesh_2)
{
    size_t vert_c = verts.size();

    for(size_t i=0; i < mesh_2.verts.size(); i++)
    {
        verts.push_back(mesh_2.verts[i]);
        normals.push_back(mesh_2.normals[i]);
        colors.push_back(mesh_2.colors[i]);
    }

    for(size_t i=0; i < mesh_2.faces.size(); i++)
    {
        faces.emplace_back(   mesh_2.faces[i].v0 + vert_c
                            , mesh_2.faces[i].v1 + vert_c
                            , mesh_2.faces[i].v2 + vert_c   );
    }
}

f3 Mesh::get_center()
{
    f3 vert_sum;
    for(auto& vert : verts)
        vert_sum += vert;

    return vert_sum / verts.size();
}

f3 Mesh::get_size()
{
    if(verts.size() == 0) return {};

    f3 min = {__FLT_MAX__, __FLT_MAX__, __FLT_MAX__};
    f3 max = {__FLT_MIN__, __FLT_MIN__, __FLT_MIN__};

    for(auto& vert : verts)
    {
        if(vert.x < min.x)
            min.x = vert.x;
        if(vert.y < min.y)
            min.y = vert.y;
        if(vert.z < min.z)
            min.z = vert.z;
        
        if(vert.x > max.x)
            max.x = vert.x;
        if(vert.y > max.y)
            max.y = vert.y;
        if(vert.z > max.z)
            max.z = vert.z;
    }

    return max - min;
}

float Mesh::get_max_radius()
{
    if(verts.size() == 0) 
        return 0.0f;
 
    float max = 0.0f;

    for(auto& vert : verts)
    {
        if(vert.norm() > max)
            max = vert.norm();
    }

    return max;
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
            normals.emplace_back(0.0f, 0.0f, 1.0f);
            colors.emplace_back(default_color);
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



void Mesh::cube_centered()
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

void Mesh::cube_origin_aligned()
{
    verts.clear();
    faces.clear();

    float SX = 1.0f;
    float SY = 1.0f;
    float SZ = 1.0f;
    

    // z = 0
    verts.emplace_back( 0.0f,   0.0f,   0.0f    );
    verts.emplace_back( SX,     0.0f,   0.0f    );
    verts.emplace_back( SX,     SY,     0.0f    );
    verts.emplace_back( 0.0f,   SY,     0.0f    );

    // z = size.z
    verts.emplace_back( 0.0f,   0.0f,   SZ      );
    verts.emplace_back( SX,     0.0f,   SZ      );
    verts.emplace_back( SX,     SY,     SZ      );
    verts.emplace_back( 0.0f,   SY,     SZ      );


    // Bottom
    faces.emplace_back( 0, 1, 2 );
    faces.emplace_back( 2, 3, 0 );
    // Top
    faces.emplace_back( 4, 5, 6 );
    faces.emplace_back( 6, 7, 4 );
    // 
    faces.emplace_back( 1, 5, 2 );
    faces.emplace_back( 2, 5, 6 );
    // 
    faces.emplace_back( 2, 6, 3 );
    faces.emplace_back( 3, 6, 7 );
    // 
    faces.emplace_back( 3, 7, 0 );
    faces.emplace_back( 0, 7, 4 );
    // 
    faces.emplace_back( 0, 4, 1 );
    faces.emplace_back( 1, 4, 5 );
}


void Mesh::aabb(AABBf aabb)
{
    cube_origin_aligned();
    scale(aabb.max-aabb.min);
    move(aabb.min);
}





void Mesh::
circle(CircleContext c_ctx)
{
    verts.clear();
    normals.clear();
    colors.clear();
    faces.clear();

    float R_MAX = 1.0f;

    uint CC  = c_ctx.CC;   // Circle count
    uint PPC = c_ctx.PPC;  // Points per circle

    #define IDX(i_ppc, i_cc) ((i_ppc) + (i_cc)*PPC)

    float radius_delta = R_MAX / CC;
    float radius = 0.0f;

    float angle_delta = PI2f / (float) PPC;
    float angle = 0.0f;

    uint i_cc  = 0;
    uint i_ppc = 0;

    // VERTS
    for(i_cc=0; i_cc < CC; i_cc++)
    {
        radius += radius_delta;

        for(i_ppc=0; i_ppc < PPC; i_ppc++)
        {
            verts.emplace_back(   radius*cos(angle)
                                , radius*sin(angle)
                                , 0.0f      );
            normals.emplace_back(0.0001f, 0.0f, 1.0f);
            colors.emplace_back(default_color);

            angle += angle_delta;
        }
    }
    verts.emplace_back(0.0f, 0.0f, 0.0f); // center
    normals.emplace_back(0.0001f, 0.0f, 1.0f);
    colors.emplace_back(default_color);

    // FACES
    for(i_cc=0; i_cc < CC-1; i_cc++)
    {
        for(uint i_ppc=0; i_ppc < PPC; i_ppc++)
        {
            uint p_i_p1_mod = (i_ppc+1) % PPC;
            faces.emplace_back(   IDX(i_ppc, i_cc)
                                , IDX(i_ppc, i_cc+1)
                                , IDX(p_i_p1_mod, i_cc)    );
            faces.emplace_back(   IDX(i_ppc, i_cc+1)
                                , IDX(p_i_p1_mod, i_cc+1)
                                , IDX(p_i_p1_mod, i_cc)    );
        }
    }


    // CENTER FACES
    for(uint i_ppc=0; i_ppc < PPC; i_ppc++)
    {
        uint p_i_p1_mod = (i_ppc+1) % PPC;
        faces.emplace_back(   IDX(i_ppc, 0)
                            , IDX(p_i_p1_mod, 0)
                            , verts.size()-1        );
    }

}

void Mesh::
circle_poly_r(CircleContext c_ctx, Polynomial<float> poly)
{
    normals.clear();

    Polynomial<float> derivative = poly.derivative();

    uint CC  = c_ctx.CC;   // Circle count
    uint PPC = c_ctx.PPC;  // Points per circle

    #define IDX(i_ppc, i_cc) ((i_ppc) + (i_cc)*PPC)

    float angle_delta = PI2f / (float) PPC;
    float angle = 0.0f;

    uint i_cc  = 0;
    uint i_ppc = 0;

    for(i_cc=0; i_cc < CC; i_cc++)
    {   
        float radius = verts[IDX(0, i_cc)].to_xy().norm();
        float dz_dr  = derivative(radius);
        float dz_dr_normal = -1.0f / dz_dr;
    
        for(i_ppc=0; i_ppc < PPC; i_ppc++)
        {
            verts[IDX(i_ppc, i_cc)].z = poly(radius);

            float dx = radius * cos(angle);
            float dy = radius * sin(angle);
            float dz = radius * dz_dr_normal;

            f3 normal = {dx, dy, dz};
            normals.emplace_back(normal.unit());

            angle += angle_delta;
        }
    }
    verts[verts.size()-1] = {0.0f, 0.0f, poly(0.0f)};
    normals.emplace_back(0.0f, 0.0f, 1.0f);

    // NORMALS

}

void Mesh::color(ColorInt color_i)
{
    colors.clear();
    Colorf3 base_color_f3 = Color::int_to_f3(color_i);

    for(size_t i=0; i < verts.size(); i++)
        colors.push_back(base_color_f3);
}



void Mesh::
poly_r(Polynomial<float> poly)
{
    normals.clear();

    Polynomial<float> derivative = poly.derivative();

    for(auto& vert : verts)
    {
        float radius = vert.to_xy().norm();
        vert.z = poly(radius);

        float dz_dr_normal  = -1.0f / derivative(radius);
        float dz = radius * dz_dr_normal;

        f3 normal = {vert.x, vert.y, dz};

        if(vert.x == 0.0f && vert.y == 0.0f)
            normal = {0.001f, 0.001f, 1.0f};

        normals.emplace_back(normal.unit());
    }
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

void Mesh::scale(f3 scale)
{
    for(auto& vert : verts)
        vert *= scale;
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


Mesh& Mesh::
move(const f3& delta)
{
    for(auto& vert : verts)
        vert += delta;
    
    return *this;
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
    colors.reserve(CTVC);


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


void Mesh::tube_color(TubeContext t_context, ColorInt base_color, ColorInt top_color)
{
    int FC  = t_context.frame_count;        // Frame count
    int CPC = t_context.circle_point_count; // Circle point count
    #define IMV(i_cp, i_f)     ((i_cp) + (i_f) * CPC)

    colors.clear();
    colors.reserve(FC * CPC + 2);

    int i_fr;   // index: frame
    int i_cp;   // index : circle point
    
    Colorf3 base_color_f3 = Color::int_to_f3(base_color);
    Colorf3 top_color_f3  = Color::int_to_f3(top_color);

    // verts
    for(i_fr=0; i_fr<FC; i_fr++)
    {
        for(i_cp=0; i_cp<CPC; i_cp++)
        {
            if(i_fr < FC * 0.8)
                colors.emplace_back(base_color_f3);
            else
            {
                int rd = rand() % 100;
                if(rd < 40)
                    colors.emplace_back(base_color_f3);
                else
                    colors.emplace_back(top_color_f3);
            }
            // f2& texco = texcos[IMV(i_cp, i_fr)];

            // texco = {0.5f, 0.5f};
        }
    }

    colors.emplace_back(base_color_f3);
    colors.emplace_back(top_color_f3);
}
