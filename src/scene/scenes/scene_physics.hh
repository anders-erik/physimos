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


class ScenePhysics
{
public:

    static void load(Physimos& physimos)
    {
        Scene3D& 		root_scene 	= physimos.manager_3D.root_scene;
        Manager3D& 	    manager_3D 	= physimos.manager_3D;
        ManagerObject& 	manager_o 	= physimos.manager_3D.manager_o;
        Manager2D& 	    manager_2D 	= physimos.manager_2D;

        root_scene.name = "scene_physics";


        // GROUND
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
        ground.id_phy = manager_3D.man_phy.push_physics(physics_ground);


        root_scene.camera.projection.set_finite();
        // root_scene.camera.projection.set_infinite();
        // root_scene.camera.projection.set_ortho();
        // root_scene.camobj.cam.orbit_tag = lampo.tag;
        // root_scene.camobj.set_free();
        root_scene.camera.object.pos = {-10.0f, 10.0f, 5.0f};
        // root_scene.camera.set_orbit_tag(phy_0.tag);
        root_scene.camera.set_orbit_center();
    }
};