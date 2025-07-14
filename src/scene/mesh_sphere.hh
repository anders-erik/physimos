#pragma once

#include "math/const.hh"
#include "math/spherical.hh"

#include "scene/mesh.hh"


struct SphereContext
{
    int lon_count;
    int lat_count;
};


class MeshSphere
{
    Mesh mesh;
    SphereContext context;

public:

    MeshSphere(SphereContext new_context)
    {
        context = new_context;
        reload();
    }
    void set_context(SphereContext new_context)
    {
        context = new_context;
    }
    Mesh get_mesh()
    {
        return mesh;
    }


    void reload()
    {
        int LAC = context.lat_count;
        int LOC = context.lon_count;

        size_t VC = LAC * LOC + 2; // vert count

        // closed tubes only for now
        mesh.verts.reserve(VC);
        mesh.normals.reserve(VC);
        mesh.colors.reserve(VC);

        size_t i_top    = VC - 2;
        size_t i_bottom = VC - 1;

        float theta_delta   = PI2f / ( (float) LOC-1 );
        float phi_delta     = PIf  / ( (float) (LAC+1) );

        float theta = 0.0f;
        float phi   = 0.0f;

        // Index Verts
        #define IV(i_lo, i_la)     ((i_lo) + (i_la) * LOC)

        int i_la, i_lo;

        for(i_la=0;i_la<LAC;i_la++)
        {
            phi = phi_delta * (float) (i_la+1);     // from +z

            for(i_lo=0;i_lo<LOC;i_lo++)
            {
                f3 vert = Spherical::to_cart(   1.0f,
                                                theta,
                                                phi     );
                
                mesh.verts.emplace_back(vert);
                mesh.normals.emplace_back(vert);
                mesh.colors.emplace_back(0.5f, 0.5f, 0.5f);

                theta = theta_delta   * (float) i_lo;   // CCW from +x

                // Print::line(Str::FL(phi, 2, Str::FloatRep::Fixed));
            }
        }


        for(i_la=0;i_la<LAC-1;i_la++)
        {
            for(i_lo=0;i_lo<LOC;i_lo++)
            {
                int i_lo_next = (i_lo+1) % LOC;

                mesh.faces.emplace_back(    IV( i_lo      , i_la   ), 
                                            IV( i_lo_next , i_la   ), 
                                            IV( i_lo      , i_la+1 )    );

                mesh.faces.emplace_back(    IV( i_lo_next , i_la   ), 
                                            IV( i_lo_next , i_la+1 ), 
                                            IV( i_lo      , i_la+1 )    );
            }
        }


        // seal top and bottom
        mesh.verts.emplace_back(0.0f, 0.0f,  1.0f); // top
        mesh.normals.emplace_back(0.0f, 0.0f,  1.0f); // top
        mesh.colors.emplace_back(0.5f, 0.5f, 0.5f); // top

        mesh.verts.emplace_back(0.0f, 0.0f, -1.0f); // bottom
        mesh.normals.emplace_back(0.0f, 0.0f, -1.0f); // bottom
        mesh.colors.emplace_back(0.5f, 0.5f, 0.5f); // bottom

        for(i_lo=0;i_lo<LOC;i_lo++)
        {
            int i_lo_next = (i_lo+1) % LOC;
            // Top
            mesh.faces.emplace_back(    IV( i_lo      , 0   ), 
                                        IV( i_lo_next , 0   ), 
                                        i_top                   );
            // bottom
            mesh.faces.emplace_back(    IV( i_lo      , LAC-1   ), 
                                        IV( i_lo_next , LAC-1   ), 
                                        i_bottom                   );
        }

        
    }
};