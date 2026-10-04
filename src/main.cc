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
#include "scene/scenes/scene_physics.hh"
#include "scene/scenes/scene_gcode.hh"

#include "lib/cmd.hh"

int main(int argc, char** argv)
{
	CLI cli {argc, argv};

	if(cli[1] == "main")
	{
		Physimos physimos { 1000, 700 };

		Scene1::load(physimos);
		// ForestPlanet::load(physimos);
		// ScenePhysics::load(physimos);
		// SceneGCode::load(physimos);

		physimos.main_loop();
	}
	else if(cli[1] == "")
	{
		Print::ln("No command selected. Running default.");
	}
	else if(cli[1] == "bash")
	{
		Print::ln("Running bash script through physimos.");

		if(cli[2] == "hello")
		{
			Print::ln("Hello!");
		}
		else // default CLI command
		{
			Print::buf("Runnig bash through popen: \n");
			Str output = CMD_popen(cli[2]);
			Print::buf(output);
		}
	}
	else if(cli[1] == "install")
	{
		Print::ln("Installing all physimos binaries locally.");

		
	}
	else if(cli[1] == "task")
	{
		Print::ln("Physimos task!");

		FILE *fp = popen("echo 'physimos task echo' \n", "r");
		if (fp == NULL)
		{
			printf("Failed to run command\n");
			return 1;
		}

		int BUF_SIZE = 128;
		char buffer[BUF_SIZE];

		// Read the output a line at a time - see cppreference on fgets
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			printf("%s", buffer);
			memset(buffer, 0, BUF_SIZE);
		}

		// Close the stream
		pclose(fp);
		return 0;
		
	}
	else
	{
		Print::ln("No supported CLI command detected.");
	}


	Print::ln("\nExiting Physimos.");
	return 0;
}




