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

#include "scene/forest_planet.hh"
#include "scene/scenes/scene_1.hh"



int main()
{
	Physimos physimos { 1000, 700 };

	Scene1::load(physimos);
	// ForestPlanet::load(physimos);

	physimos.main_loop();


	return 0;
}




