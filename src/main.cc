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

#include "rend/rend_manager.hh"

int main()
{

	Physimos physimos { 800, 600 };

	Scene3D& root_scene = ManagerScene::get_root_scene_mut();
	root_scene.set_name("Forever Root");


	// GROUND GRASS
	model::ModelT ground_grass;
    model::model_generate_ground(ground_grass.mesh);
    model::model_center(ground_grass.mesh);
	model::model_translate(ground_grass.mesh, {0.0f, 0.0f, -1.5f});
    ground_grass.texture = opengl::Textures::Grass;
	root_scene.push_texture_model(ground_grass);



	// GROUND CHECKER
	model::ModelT ground_checker;
	model::model_generate_ground(ground_checker.mesh);
    model::model_center(ground_checker.mesh);
	model::model_translate(ground_checker.mesh, {5.0f, 0.0f, -1.4f});
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


	// GRAPH SCENE2D
	scene::Scene2D scene2D { {100, 100} };
	scene2D.get_camera().set_width(3);
	scene2D.get_camera().pan({1.5f, 1.5f});

    R2<float> domain = {-2.0f, 2.0f};
    // Polynomial<float> polynomial {{0, 0, 0, 1}};
	Polynomial<float> polynomial {{1, 0, 2, 1, -1}};
	polynomial.x_stretch = 0.5f;
	polynomial.x_shift = 1.0f;
	polynomial.y_stretch = 0.3f;
	polynomial.y_shift = -1.0f;
    Graph2D<float> graph {41, domain, polynomial};
	graph.update();
	scene2D.graphs.push_back(graph);
	
	SID sid_2D = ManagerScene::push_scene2D(scene2D);

	// for(size_t i = 0; i < 20; i++)
	// {
	// 	std::cout << graph.x[i] << ", " << graph.y[i] << std::endl;
	// }

	// QuadO
	SQuad s_q_texture;
	s_q_texture.texture_id = Rend::Manager::get_renderer_scene2D().get_scene_fb_texture_id(sid_2D);
	s_q_texture.sid = sid_2D;
	SQuadO& quado = ObjectManager::push_quado(s_q_texture);
	root_scene.push_object(quado.object);


	// Guarantee one proper rend call
	Rend::Manager::get_renderer_scene2D().render_scene_framebuffer(sid_2D);


	physimos.main_loop();

	return 0;
}




