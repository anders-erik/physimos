#include <iostream>

// LEGACY
#include "logger.hh"


#include "lib/process.hh"
#include "conductor.hh"

#include "opengl/shader.hh"
#include "opengl/program.hh"


int main()
{
	
	std::cout << std::endl << "Consistent incremental improvement is the fundamental action of anything worthwhile. " << std::endl;
	std::cout              << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << std::endl << std::endl;


	// Make sure physimos is either installed or launched from repo. If not, terminate.
	// TODO: Enable selection of root directory instead of automatic exit! Or just create a small popup notifying user of problem.
	physimos_root_dir_or_die();


	// MISC
	srand(0);
	logger::init();
	process_info_init();


	conductor_rouse();

	// opengl::Shader ui_color_vert {"ui/color.vert"};
	// ui_color_vert.compile();
	// opengl::Shader ui_color_frag {"ui/color.frag"};
	// ui_color_frag.compile();

	opengl::ShaderProgram ui_color = {"ui/color"};
	
	conductor_conduct();
	

	return 0;
}





