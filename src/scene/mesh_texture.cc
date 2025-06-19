
#include "mesh.hh"
#include "mesh_texture.hh"

unsigned int MeshTexture::vert_size_bytes()
{
    return verts.size() * sizeof(VertexTexture);
}


void MeshTexture::create_cube()
{

}

void MeshTexture::create_quad()
{
    verts.clear();
    
    float texture_size = 1.0f;

    // Clock-wise 

    // 0
    verts.emplace_back( 0.0f,
                        0.0f);
    // 1
    verts.emplace_back( texture_size,
                        0.0f);
    // 2
    verts.emplace_back( texture_size,
                        texture_size);
    // 3
    verts.emplace_back( 0.0f,
                        texture_size);

}
