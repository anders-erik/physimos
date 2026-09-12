#pragma once

#include <stdio.h>
#include <cstring>

#include "lib/cli.hh"

Str CMD_popen(Str bash_string)
{
	const char* str_to_run = bash_string.to_c_str();
	size_t str_to_run_length = strlen(str_to_run);

	FILE* file = popen(str_to_run, "r");

	char buf[4096];

	// while (fgets(buf, sizeof(buf), file)) {
	// while (fread(buf, sizeof(buf), 10, file)) {
	// 	// printf("%s", buf);
	// }

	fread(buf, 1, sizeof(buf), file);

	return Str{buf};
}


int cmd(int argc, char** argv)
{
	
	CLI cli {argc, argv};

	

	return 0;
}
