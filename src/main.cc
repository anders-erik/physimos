#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>


#include "scene/manager.hh"

#include "conductor/conductor2D.hh"



f2 window_size = {800.0f, 600.0f};




int main()
{

	f2 conductor_window_size = {800.0f, 600.0f};

	Conductor2D conductor2D { conductor_window_size };

	Scene3D& root_scene = ManagerScene::get_root_scene_mut();

	


	conductor2D.main_loop();

	return 0;
}




