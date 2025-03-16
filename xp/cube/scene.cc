
#include "scene.hh"

#include "model.hh"
#include "math.hh"

namespace xpcube {

RendererModel renderer_model;

Model cube;




bool scene_init(){

    renderer_model.init();

    // Vertex v0 = cube.mesh.vertices.emplace_back(Vertex(f3{0.0f, 0.0f, 0.0f}));
    // cube.mesh.vertices.emplace_back(Vertex(f3{1.0f, 1.0f, 0.0f}));
    // cube.mesh.vertices.emplace_back(Vertex(f3{0.0f, 1.0f, 0.0f}));
    Vertex v0; //= cube.mesh.vertices.emplace_back(Vertex());
    v0.pos.x = 0.0f;
    v0.pos.y = 0.0f;
    v0.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v0);

    Vertex v1; //= cube.mesh.vertices.emplace_back(Vertex());
    v1.pos.x = 1.0f;
    v1.pos.y = 1.0f;
    v1.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v1);


    Vertex v2; //= cube.mesh.vertices.emplace_back(Vertex());
    v2.pos.x = 0.0f;
    v2.pos.y = 1.0f;
    v2.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v2);

    cube.mesh.faces.emplace_back(TriangleFaceIndeces{0, 1, 2});

    
    renderer_model.create_render_context(cube);
    

    return true;
}


void scene_render(){

    renderer_model.render(cube);
}



}
