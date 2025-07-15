#pragma once


#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>



#include "conductor/conductor2D.hh"


#include "scene/mesh.hh"

#include "scene/manager_object.hh"
#include "scene/manager_property.hh"

// #include "io/json/examples/jfile.hh"
#include "io/jfile/jmesh.hh"
#include "script/script_mesh.hh"


class Scene1
{
public:

    static void load(Physimos& physimos)
    {
        Scene3D& 		root_scene 	= physimos.manager_3D.root_scene;
        Manager3D& 	    manager_3D 	= physimos.manager_3D;
        ManagerObject& 	manager_o 	= physimos.manager_3D.manager_o;
        Manager2D& 	    manager_2D 	= physimos.manager_2D;

        root_scene.name = "Scene_1";


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



        // GRAPH SCENE2D
        Scene2D scene2D { {100, 100} };
        scene2D.manager_2D = &manager_2D;
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
        
        SID graph_sid = manager_2D.push_scene2D(scene2D, physimos.rendererScene2D);



        // QuadO
        Object& squad_object = manager_o.new_object();
        squad_object.name = "quado_1";
        squad_object.mesh.quad();
        squad_object.mesh.center();
        squad_object.tag.type = TagO::Type::Quad;
        squad_object.pos.x = 0.0f;
        squad_object.pos.z = 1.0f;
        root_scene.quad_tags.push_back(squad_object.tag);

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

            // root_scene.tagos.push_back(mesh_obj.tag);
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

            // root_scene.tagos.push_back(mesh_obj.tag);
        }


        // TOADSTOOL
        // Object& toadstool = manager_o.new_object();
        // toadstool.tag.type = TagO::Base;
        // toadstool.name = "toadstool";
        // toadstool.pos = {2.0f, 0.0f, 1.0f};
        // toadstool.mesh = ForestPlanet::toadstool_mesh_1();
        // root_scene.tagos.push_back(toadstool.tag);


        // // TOADSTOOL MERGE
        // Object& toadst_merge = manager_o.new_object();
        // toadst_merge.tag.type = TagO::Base;
        // toadst_merge.name = "toadst_merged";
        // toadst_merge.pos = {1.0f, -3.0f, 0.0f};
        // toadst_merge.mesh = ForestPlanet::toadstool_mesh_2();
        // root_scene.tagos.push_back(toadst_merge.tag);


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
        root_scene.lamp_tags.push_back(lampo.tag);

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

        // PHYSICS - STATIC CUBE
        Object& phy_0 = manager_o.new_object();
        phy_0.tag.type = TagO::Base;
        phy_0.rend_cxt.set_shading_wireframe();
        phy_0.mesh.cube_centered();
        phy_0.mesh.center();
        phy_0.mesh.scale(3.0f);
        phy_0.name = "phy_0";
        phy_0.pos = {10.0f, -1.0f, 0.0f};
        root_scene.tagos.push_back(phy_0.tag);

        Physics physics_0;
        physics_0.set_static_aabb(	phy_0.pos,
                                    phy_0.mesh.get_size() / 2);
        // physics_0.coll_bits = COLL_AABB;
        // physics_0.aabb.half_size = phy_0.mesh.get_size() / 2;
        phy_0.pyh_tag = manager_3D.manager_p.push_physics(physics_0);


        // PHYSICS - SHEET
        Object& phy_1 = manager_o.new_object();
        phy_1.tag.type = TagO::Base;
        // phy_1.mesh.sheet({2, 2});
        phy_1.rend_cxt.set_shading_wireframe();
        phy_1.mesh.cube_centered();
        phy_1.mesh.scale({2.0f, 2.0f, 0.1f});
        phy_1.mesh.center();
        phy_1.name = "phy_1";
        phy_1.pos = {5.5f, 2.0f, 0.0f};
        root_scene.tagos.push_back(phy_1.tag);

        Physics physics_1;
        physics_1.set_static_aabb(	phy_1.pos,
                                    phy_1.mesh.get_size() / 2);
        // physics_1.coll_bits = COLL_AABB;
        // physics_1.aabb.half_size = phy_1.mesh.get_size() / 2;
        phy_1.pyh_tag = manager_3D.manager_p.push_physics(physics_1);


        Object& phyo_sph = manager_o.new_object();
        phyo_sph.tag.type = TagO::Base;
        phyo_sph.rend_cxt.set_shading_wireframe();
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
        phyo_sph_2.rend_cxt.set_shading_wireframe();
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
        phyo_dyn_sph_1.rend_cxt.set_shading_wireframe();
        phyo_dyn_sph_1.mesh.cube_centered();
        phyo_dyn_sph_1.mesh.center();
        phyo_dyn_sph_1.name = "phyo_dyn_sph_1";
        root_scene.tagos.push_back(phyo_dyn_sph_1.tag);

        Physics physics_dyn_sph_1;
        physics_dyn_sph_1.set_dynamic_sphere(	{3.0f, 1.0f, 2.0f}, 
                                            phyo_dyn_sph_1.mesh.get_max_radius()	);
        phyo_dyn_sph_1.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_sph_1);


        Object& phyo_dyn_sph_2 = manager_o.new_object();
        phyo_dyn_sph_2.tag.type = TagO::Base;
        phyo_dyn_sph_2.rend_cxt.set_shading_wireframe();
        phyo_dyn_sph_2.mesh.cube_centered();
        phyo_dyn_sph_2.mesh.center();
        phyo_dyn_sph_2.name = "phyo_dyn_sph_2";
        root_scene.tagos.push_back(phyo_dyn_sph_2.tag);

        Physics physics_dyn_sph_2;
        physics_dyn_sph_2.set_dynamic_sphere(	{7.7f, 0.0f, 3.0f}, 
                                                phyo_dyn_sph_2.mesh.get_max_radius()	);
        phyo_dyn_sph_2.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_sph_2);


        Object& phyo_dyn_cube_1 = manager_o.new_object();
        phyo_dyn_cube_1.tag.type = TagO::Base;
        phyo_dyn_cube_1.rend_cxt.set_shading_wireframe();
        phyo_dyn_cube_1.mesh.cube_centered();
        phyo_dyn_cube_1.name = "phyo_dyn_cube_1";
        root_scene.tagos.push_back(phyo_dyn_cube_1.tag);

        Physics physics_dyn_cube_1;
        physics_dyn_cube_1.set_dynamic_aabb(	{5.0f, -2.0f, 2.0f}, 
                                                phyo_dyn_cube_1.mesh.get_size() / 2	);
        phyo_dyn_cube_1.pyh_tag = manager_3D.manager_p.push_physics(physics_dyn_cube_1);


        // GRASS
        Object& grass = manager_o.new_object();
        grass.tag.type = TagO::Base;
        grass.mesh = ForestPlanet::grass_1();        
        grass.name = "grass";
        grass.rend_cxt.instance_ctx.enabled = true;
        grass.rend_cxt.instance_ctx.instance_count = 100;
        root_scene.tagos.push_back(grass.tag);
        
        // CAMERA
        // root_scene.camobj.view = cam;
        root_scene.camera.perspective.set_fov(800, 600);
        // root_scene.camobj.cam.orbit_tag 	= lampo.tag;
        // root_scene.camobj.set_free();
        root_scene.camera.object.pos = {-10.0f, 10.0f, 5.0f};
        // root_scene.camera.set_orbit_tag(phy_0.tag);
        root_scene.camera.set_orbit_center();
    }
};