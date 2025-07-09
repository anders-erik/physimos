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
#include "scene/manager_property.hh"

// #include "io/json/examples/jfile.hh"
#include "io/jfile/jmesh.hh"
#include "script/script_mesh.hh"

int main()
{

	Physimos physimos { 1000, 700 };

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
    cube.transform.pos.x = 8.0f;
	root_scene.color_models.push_back(cube);


	// GROUND - PLACED FIRST AS I HAVENT ENABLED DEPTH TEST IN ID FB
	Object& ground = manager_o.new_object();
	ground.tag.type = TagO::Base;
	ground.mesh.sheet({40, 2});
	ground.mesh.color(0x222222);
	ground.mesh.center();
	ground.name = "ground";
	ground.pos = {0.0f, 0.0f, -1.6f};
	root_scene.tagos.push_back(ground.tag);

	Physics physics_ground;
	physics_ground.set_static_aabb(	ground.pos,
									ground.mesh.get_size() / 2);
	ground.pyh_tag = manager_3D.manager_p.push_physics(physics_ground);


	// FIRST PURE MESH
	Object& object_base = manager_o.new_object();
	object_base.mesh.cube_centered();
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
		mesh_obj.name = "jmesh";
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
		mesh_obj.name = "mesh_script";
		mesh_obj.mesh = mesh_script.consume_value();
		// mesh_obj.mesh.sheet();
		// mesh_obj.mesh.center();

		root_scene.tagos.push_back(mesh_obj.tag);
	}


	// TOADSTOOL
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
	toadstool.mesh.tube_color(t_context, 0xffffff, 0xff0000);


	// TOADSTOOL MERGE
	Object& toadst_merge = manager_o.new_object();
	toadst_merge.tag.type = TagO::Base;
	toadst_merge.name = "toadst_merged";
	toadst_merge.mesh.circle({3, 10});
	toadst_merge.mesh.color(0xff0000);

	Mesh dot_mesh;
	dot_mesh.circle({1, 8});
	dot_mesh.scale(0.05f);

	for (size_t i = 0; i < 40; i++)
	{
		srand(i);
		float rd_x = ((float)(rand() % 1000) / 700.f) - 0.7f;
		srand(100*i);
		float rd_y = ((float)(rand() % 1000) / 700.f) - 0.7f;

		dot_mesh.center();
		dot_mesh.move({rd_x, rd_y, 0.001f});
		// dot_mesh.get_center().print("Vert center");
		toadst_merge.mesh.merge(dot_mesh);
	}

	Polynomial<float> poly_top {{0.2, 0, -0.2}};
	toadst_merge.mesh.poly_r(poly_top);

	Mesh bottom_mesh;
	bottom_mesh.tube({20, 8});
	bottom_mesh.color(0xffffff);
	bottom_mesh.scale_z(2.0f);
	bottom_mesh.move_z(-1.0f);
	bottom_mesh.tube_poly_z(t_context, {{1, 0, 2, 1, -1}});
	bottom_mesh.center();
	bottom_mesh.scale(0.3f);
	bottom_mesh.move_z(-0.28f);

	toadst_merge.mesh.merge(bottom_mesh);
	
	toadst_merge.pos = {1.0f, -3.0f, 0.0f};
	root_scene.tagos.push_back(toadst_merge.tag);

	

	// manager_3D.window_scene->q_1000.rotate(f3::Z(), 0.785f);
	// manager_3D.window_scene->q_1000.rotate(f3::X(), 1.57f);


	// LAMP
	Object& lampo = manager_o.new_object();
	lampo.tag.type = TagO::Lamp;
	lampo.pos = {3.0f, 0.0f, 2.0f};
	lampo.mesh.cube_centered();
	lampo.mesh.scale(0.2f);
	lampo.name = "lamp_1";
	Lamp lamp;
	lamp.light_color = {0.8f, 0.6f, 0.6f};
	lampo.tagp = manager_3D.manager_p.push_lamp(lamp);
	root_scene.tagos.push_back(lampo.tag);

	// CAMERA OBJECT
	Object& cam_o = manager_o.new_object();
	// cam_o.tag.type = TagO::Base;
	cam_o.tag.type = TagO::Camera;
	cam_o.pos = {-10.0f, -1.0f, 0.0f};
	// cam_o.rot.rotate(f3::Z(), PIHf);
	// cam_o.rot.rotate(f3::X(), -PIHf);
	cam_o.mesh.cube_centered();
	cam_o.mesh.scale({0.5f, 0.1f, 0.2f});
	cam_o.name = "camera_1";
	CameraView cam;
	// cam.orbit_tag = lampo.tag;
	cam_o.tagp = manager_3D.manager_p.push_camera(cam);
	root_scene.tagos.push_back(cam_o.tag);


	// PHYSICS BODY
	Object& phy_0 = manager_o.new_object();
	phy_0.tag.type = TagO::Base;
	phy_0.mesh.cube_centered();
	phy_0.mesh.center();
	phy_0.name = "phy_0";
	phy_0.pos = {3.0f, 1.0f, 0.0f};
	root_scene.tagos.push_back(phy_0.tag);

	Physics physics_0;
	physics_0.set_static_aabb(	phy_0.pos,
								phy_0.mesh.get_size() / 2);
	// physics_0.coll_bits = COLL_AABB;
	// physics_0.aabb.half_size = phy_0.mesh.get_size() / 2;
	phy_0.pyh_tag = manager_3D.manager_p.push_physics(physics_0);


	Object& phy_1 = manager_o.new_object();
	phy_1.tag.type = TagO::Base;
	phy_1.mesh.sheet({1, 2});
	phy_1.mesh.center();
	phy_1.name = "phy_1";
	phy_1.pos = {4.5f, 1.0f, 0.0f};
	root_scene.tagos.push_back(phy_1.tag);

	Physics physics_1;
	physics_1.set_static_aabb(	phy_1.pos,
								phy_1.mesh.get_size() / 2);
	// physics_1.coll_bits = COLL_AABB;
	// physics_1.aabb.half_size = phy_1.mesh.get_size() / 2;
	phy_1.pyh_tag = manager_3D.manager_p.push_physics(physics_1);


	Object& phyo_sph = manager_o.new_object();
	phyo_sph.tag.type = TagO::Base;
	phyo_sph.mesh.sheet({1, 2});
	phyo_sph.mesh.center();
	phyo_sph.name = "phy_sph";
	phyo_sph.pos = {4.5f, -2.0f, 0.0f};
	root_scene.tagos.push_back(phyo_sph.tag);

	Physics physics_sph;
	physics_sph.set_static_sphere(	phyo_sph.pos, 
									phyo_sph.mesh.get_max_radius()	);
	phyo_sph.pyh_tag = manager_3D.manager_p.push_physics(physics_sph);

	Object& phyo_sph_2 = manager_o.new_object();
	phyo_sph_2.tag.type = TagO::Base;
	phyo_sph_2.mesh.cube_centered();
	phyo_sph_2.mesh.center();
	phyo_sph_2.name = "phy_sph_2";
	phyo_sph_2.pos = {6.0f, 0.0f, 0.0f};
	root_scene.tagos.push_back(phyo_sph_2.tag);

	Physics physics_sph_2;
	physics_sph_2.set_static_sphere(	phyo_sph_2.pos, 
										phyo_sph_2.mesh.get_max_radius()	);
	phyo_sph_2.pyh_tag = manager_3D.manager_p.push_physics(physics_sph_2);


	Object& phyo_dyn_sph_1 = manager_o.new_object();
	phyo_dyn_sph_1.tag.type = TagO::Base;
	phyo_dyn_sph_1.mesh.cube_centered();
	phyo_dyn_sph_1.mesh.center();
	phyo_dyn_sph_1.name = "phyo_dyn_sph_1";
	root_scene.tagos.push_back(phyo_dyn_sph_1.tag);

	Physics physics_dyn_sph_1;
	physics_dyn_sph_1.set_dynamic_sphere(	{3.0f, 1.0f, 2.0f}, 
										phyo_dyn_sph_1.mesh.get_max_radius()	);
	// phyo_dyn_sph_1.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_sph_1);


	Object& phyo_dyn_sph_2 = manager_o.new_object();
	phyo_dyn_sph_2.tag.type = TagO::Base;
	phyo_dyn_sph_2.mesh.cube_centered();
	phyo_dyn_sph_2.mesh.center();
	phyo_dyn_sph_2.name = "phyo_dyn_sph_2";
	root_scene.tagos.push_back(phyo_dyn_sph_2.tag);

	Physics physics_dyn_sph_2;
	physics_dyn_sph_2.set_dynamic_sphere(	{5.9f, 0.0f, 2.0f}, 
											phyo_dyn_sph_2.mesh.get_max_radius()	);
	phyo_dyn_sph_2.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_sph_2);


	Object& phyo_dyn_cube_1 = manager_o.new_object();
	phyo_dyn_cube_1.tag.type = TagO::Base;
	phyo_dyn_cube_1.mesh.cube_centered();
	phyo_dyn_cube_1.name = "phyo_dyn_cube_1";
	root_scene.tagos.push_back(phyo_dyn_cube_1.tag);

	Physics physics_dyn_cube_1;
	physics_dyn_cube_1.set_dynamic_aabb(	{5.0f, -2.0f, 2.0f}, 
											phyo_dyn_cube_1.mesh.get_size() / 2	);
	// phyo_dyn_cube_1.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_cube_1);



	
	// CAMERA
	// root_scene.camobj.view = cam;
	root_scene.camobj.perspective.set_fov(800, 600);
	// root_scene.camobj.cam.orbit_tag 	= lampo.tag;
	// root_scene.camobj.set_free();
	root_scene.camobj.object.pos = {-10.0f, 10.0f, 5.0f};
	root_scene.camobj.set_orbit_tag(phyo_sph_2.tag);


	physimos.main_loop();

	return 0;
}




