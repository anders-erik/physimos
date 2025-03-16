
#include "scene.hh"

#include "model.hh"
#include "math.hh"

namespace xprubik {

RendererModel renderer_model;

Camera camera;

Model cube;


void Camera::set_matrices(){
    m4f4 indentity;

    view_mat = indentity;
    f3 negative_camera_pos;
    negative_camera_pos.x = -camera.transform.pos.x;
    negative_camera_pos.y = -camera.transform.pos.y;
    negative_camera_pos.z = -camera.transform.pos.z;
    view_mat.translate(negative_camera_pos);
    



}

bool scene_init(){

    renderer_model.init();

    // camera.transform.rot.z = 0.5;
    camera.transform.pos.x = 0.5;
    camera.transform.pos.y = 0.5;
    camera.transform.pos.z = 0.0;

    

    Vertex v0;
    // v0.pos.x = 0.0f;
    // v0.pos.y = 0.0f;
    // v0.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v0);

    Vertex v1;
    v1.pos.x = 1.0f;
    v1.pos.y = 1.0f;
    // v1.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v1);


    Vertex v2;
    // v2.pos.x = 0.0f;
    v2.pos.y = 1.0f;
    // v2.pos.z = 0.0f;
    cube.mesh.vertices.push_back(v2);

    cube.mesh.faces.emplace_back(TriangleFaceIndeces{0, 1, 2});


    renderer_model.create_render_context(cube);
    

    return true;
}


void scene_render(){
    // camera
    camera.set_matrices();
    renderer_model.set_camera_uniforms(camera.view_mat, camera.perspective_mat);

    renderer_model.render(cube);
}



}
