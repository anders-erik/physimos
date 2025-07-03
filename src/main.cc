#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>



#include "conductor/conductor2D.hh"

#include "scene/model.hh"

#include "scene/mesh.hh"

#include "scene/manager_object.hh"

// #include "io/json/examples/jfile.hh"
#include "io/jfile/jmesh.hh"
#include "script/script_mesh.hh"

int main()
{

	Physimos physimos { 800, 600 };

	Manager3D& manager_3D = physimos.manager_3D;
	Scene3D& root_scene = manager_3D.root_scene;
	ManagerObject& manager_o = manager_3D.manager_o;

	root_scene.name = "Forever Root";


	// GROUND GRASS
	model::ModelT ground_grass;
    model::model_generate_ground(ground_grass.mesh);
    model::model_center(ground_grass.mesh);
	model::model_translate(ground_grass.mesh, {0.0f, 0.0f, -1.5f});
    ground_grass.texture = opengl::Textures::Grass;
	root_scene.texture_models.push_back(ground_grass);



	// GROUND CHECKER
	model::ModelT ground_checker;
	model::model_generate_ground(ground_checker.mesh);
    model::model_center(ground_checker.mesh);
	model::model_translate(ground_checker.mesh, {5.0f, 0.0f, -1.4f});
    ground_checker.texture = opengl::Textures::Checker2x2;
	root_scene.texture_models.push_back(ground_checker);



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
	root_scene.texture_models.push_back(tube);



	// COLOR CUBE
	model::ModelColor cube;
	model_add_cube_mesh(cube.mesh);
    // renderer_model_color.create_render_context(model_render_context, model);
    cube.transform.pos.x = 5.0f;
	root_scene.color_models.push_back(cube);



	// FIRST PURE MESH
	Object& object_base = manager_o.new_object();
	object_base.mesh.cube();
	object_base.tag.type = TagO::Type::Base;
	root_scene.tagos.push_back(object_base.tag);


	// GRAPH SCENE2D
	Scene2D scene2D { {100, 100} };
	scene2D.manager_2D = &physimos.manager_2D;
	scene2D.get_camera().set_width(3);
	scene2D.get_camera().pan({1.5f, 1.5f});

    R2<float> domain = {-2.0f, 2.0f};
    // Polynomial<float> polynomial {{0, 0, 0, 1}};
	Polynomial<float> polynomial {{1, 0, 2, 1, -1}};
	polynomial.x_stretch = 0.5f;
	polynomial.x_shift = 1.0f;
	polynomial.y_stretch = 0.3f;
	polynomial.y_shift = -1.0f;
    Graph2D<float> graph {domain, 41, polynomial};
	graph.update();
	scene2D.graphs.push_back(graph);
	
	SID graph_sid = physimos.manager_2D.push_scene2D(scene2D, physimos.rendererScene2D);



	// QuadO
	Object& squad_object = manager_o.new_object();
	squad_object.name = "quado_1";
	squad_object.mesh.quad();
	squad_object.mesh.center();
	squad_object.tag.type = TagO::Type::Quad;
	squad_object.pos.x = 0.0f;
	squad_object.pos.z = 1.0f;
	root_scene.tagos.push_back(squad_object.tag);

	Quad quad;
	// quad.texture_id = Rend::Manager::get_renderer_scene2D().get_scene_fb_texture_id(graph_sid);
	quad.texture_id = physimos.rendererScene2D.get_scene_fb_texture_id(graph_sid);
	quad.sid_2D = graph_sid;

	manager_3D.manager_q.quados.emplace_back(	squad_object.tag, 
												quad				);


	// JFILE LOADING
	ResMove<Mesh> mesh_from_file = JMesh::load_mesh_core("resources/scene/toadstool_1.json");
	if(mesh_from_file.has_value())
	{
		Object& mesh_obj = manager_o.new_object();
		mesh_obj.tag.type = TagO::Base;
		mesh_obj.mesh = mesh_from_file.consume_value();
		// mesh_obj.mesh.sheet();
		// mesh_obj.mesh.center();

		root_scene.tagos.push_back(mesh_obj.tag);
	}

	// JMesh jmesh;
	// Mesh mesh_quad;

	// S_MESH -- Mesh script
	ResMove<Mesh> mesh_script = ScriptMesh::run("resources/mesh/toadstool.smesh");
	if(mesh_script.has_value())
	{
		Object& mesh_obj = manager_o.new_object();
		mesh_obj.tag.type = TagO::Base;
		mesh_obj.mesh = mesh_script.consume_value();
		// mesh_obj.mesh.sheet();
		// mesh_obj.mesh.center();

		root_scene.tagos.push_back(mesh_obj.tag);
	}


	// TUBE
	Object& toadstool = manager_o.new_object();
	toadstool.tag.type = TagO::Base;
	toadstool.name = "toadstool";
	root_scene.tagos.push_back(toadstool.tag);
	toadstool.pos = {2.0f, 0.0f, 1.0f};
	// mesh
	TubeContext t_context {20, 8};
	toadstool.mesh.tube(t_context);
	toadstool.mesh.scale_z(3.0f);
	toadstool.mesh.move_z(-1.0f);
	Polynomial<float> poly_toadstool {{1, 0, 2, 1, -1}};
	toadstool.mesh.tube_poly_z(t_context, poly_toadstool);
	toadstool.mesh.center();
	toadstool.mesh.scale(0.1f);

	// manager_3D.window_scene->q_1000.rotate(f3::Z(), 0.785f);
	// manager_3D.window_scene->q_1000.rotate(f3::X(), 1.57f);

	physimos.main_loop();

	

	return 0;
}




