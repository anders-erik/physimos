#pragma once

#include "lib/print.hh"
#include "lib/arr.hh"


class CLI
{
public:

	uint argc;
	Arr<Str> argv;

	CLI(int _argc, char** _argv)
	{
		if(_argc < 1)
		{
			println("ERROR: CLI argc is < 1.");
			return;
		}

		argc = (uint)_argc;
		
		for(uint i = 0; i < argc; i++)
		{
			argv.push_back(_argv[i]);
		}
	}

	Str operator[](uint _argument_index)
	{
		return argv[_argument_index];
	}

	void print()
	{
		Print::ln("CLI::print ");
		for(uint i = 0; i < argc; i++)
		{
			Print::buf("    ");
			Print::buf(Str::UI(i));
			Print::buf(": ");
			Print::ln(argv[i]);
		}
	}
};