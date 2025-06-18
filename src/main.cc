#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>


#include "scene/manager.hh"

#include "conductor/conductor2D.hh"

#include "model/model.hh"

#include "scene/mesh.hh"
#include "scene/object_manager.hh"


int main()
{

	f2 window_size = {800.0f, 600.0f};

	Physimos physimos { window_size };

	Scene3D& root_scene = ManagerScene::get_root_scene_mut();
	root_scene.set_name("Forever Root");


	// GROUND GRASS
	model::ModelT ground_grass;
    model::model_generate_ground(ground_grass.mesh);
    model::model_center(ground_grass.mesh);
	model::model_translate(ground_grass.mesh, {0.0f, 0.0f, 0.0f});
    ground_grass.texture = opengl::Textures::Grass;
	root_scene.push_texture_model(ground_grass);



	// GROUND CHECKER
	model::ModelT ground_checker;
	model::model_generate_ground(ground_checker.mesh);
    model::model_center(ground_checker.mesh);
	model::model_translate(ground_checker.mesh, {5.0f, 0.0f, 0.1f});
    ground_checker.texture = opengl::Textures::Checker2x2;
	root_scene.push_texture_model(ground_checker);



	// TUBE
	model::TubeContext tube_context = {
		1.0f,   //  float radius;
		6,      //  int frame_point_count;
		10,     //  int frame_count;
		1.0,    //  float frame_gap;
		0,      //  int vertex_count_target;
	};
	model::ModelT tube;
	model_generate_tube(tube.mesh, tube_context);
    model_center(tube.mesh);
    model_scale(tube.mesh, 0.5f);
    model_translate(tube.mesh, {-3.0f, 0.0f, 2.0f});
    tube.texture = opengl::Textures::Colors;
	root_scene.push_texture_model(tube);



	// COLOR CUBE
	model::ModelColor cube;
	model_add_cube_mesh(cube.mesh);
    // renderer_model_color.create_render_context(model_render_context, model);
    cube.transform.pos.x = 5.0f;
	root_scene.push_color_model(cube);



	// FIRST PURE MESH
	Mesh mesh;
	mesh.create_cube();
	MeshO& mesho = ObjectManager::push_mesho(mesh);
	root_scene.push_object(mesho.object);



	physimos.main_loop();

	return 0;
}




