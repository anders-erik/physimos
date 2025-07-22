#pragma once

#include "math/polynomial.hh"

#include "conductor/conductor2D.hh"

#include "scene/mesh_sphere.hh"

class ForestPlanet
{
public:

    static void load(Physimos& physimos)
    {
        Scene3D& 		root_scene 	= physimos.manager_3D.root_scene;
        Manager3D& 	    manager_3D 	= physimos.manager_3D;
        ManagerObject& 	manager_o 	= physimos.manager_3D.manager_o;
        Manager2D& 	    manager_2D 	= physimos.manager_2D;

        // root_scene

        root_scene.name = "ForestPlanet";

        MeshSphere planet_sphere({10, 10});

        // GROUND - PLACED FIRST AS I HAVENT ENABLED DEPTH TEST IN ID FB
        Object& planet = manager_o.new_object();
        planet.tag.type = TagO::Base;
        planet.mesh = planet_sphere.get_mesh();
        planet.name = "planet";
        planet.pos = {0.0f, 0.0f, 0.0f};
        root_scene.tagos.push_back(planet.tag);


        Physics physics_planet;
        physics_planet.set_static_sphere(	planet.pos,
                                            1.0f        ); // Radius
        planet.pyh_tag = manager_3D.manager_p.push_physics(physics_planet);


        // TOADSTOOL
        Object& toadstool = manager_o.new_object();
        toadstool.tag.type = TagO::Base;
        toadstool.name = "toadstool";
        toadstool.pos = {5.0f, 0.0f, 0.0f};
        toadstool.mesh = toadstool_mesh_2();
        root_scene.tagos.push_back(toadstool.tag);


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

        // CAMERA
        // root_scene.camobj.view = cam;
        root_scene.camera.projection.set_viewport(800, 600);
        // root_scene.camobj.cam.orbit_tag 	= lampo.tag;
        // root_scene.camobj.set_free();
        root_scene.camera.object.pos = {-10.0f, 10.0f, 5.0f};
        root_scene.camera.set_orbit_tag(planet.tag);
        root_scene.camera.set_orbit_center();
    }


    static Mesh grass_1()
    {
        Mesh mesh;
        mesh.sheet({1.0f, 2});
        mesh.center();
        mesh.shear_y(3.0f);
        mesh.scale(0.1f);
        mesh.color(0x00ff00);
        return mesh;
    }

    static Mesh toadstool_mesh_1()
    {
        Mesh mesh;
        TubeContext t_context {20, 8};
        mesh.tube(t_context);
        mesh.scale_z(3.0f);
        mesh.move_z(-1.0f);
        Polynomial<float> poly_toadstool {{1, 0, 2, 1, -1}};
        mesh.tube_poly_z(t_context, poly_toadstool);
        mesh.center();
        mesh.scale(0.1f);
        mesh.tube_color(t_context, 0xffffff, 0xff0000);
        return mesh;
    }

    static Mesh toadstool_mesh_2()
    {
        Mesh mesh;

        mesh.circle({3, 10});
        mesh.color(0xff0000);

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
            mesh.append(dot_mesh);
        }

        Polynomial<float> poly_top {{0.2, 0, -0.2}};
        mesh.poly_r(poly_top);

        Mesh bottom_mesh;
        TubeContext bottom_tube_ctx {20, 10};
        bottom_mesh.tube(bottom_tube_ctx);
        bottom_mesh.color(0xffffff);
        bottom_mesh.scale_z(2.0f);
        bottom_mesh.move_z(-1.0f);
        bottom_mesh.tube_poly_z(bottom_tube_ctx, {{1, 0, 2, 1, -1}});
        bottom_mesh.center();
        bottom_mesh.scale(0.3f);
        bottom_mesh.move_z(-0.28f);

        mesh.append(bottom_mesh);

        return mesh;
    }

};