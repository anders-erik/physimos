#pragma once

#include "window/auxevent.hh"



struct MouseGrab
{
	enum State
	{
		EMPTY		= 0x00,
		PUI	 		= 0x01,
		QUAD	 	= 0x02,
		SCENE	 	= 0x04,
	} state = EMPTY;

	void update(State subsystem, InputResponse response)
	{
		if(response.grabbed_mouse())
			state = subsystem;
		else
			state = EMPTY;
	}

	void release()
	{
		state = EMPTY;
	}

	bool pui()
	{
		return state == PUI;
	}

	bool quad()
	{
		return state == QUAD;
	}

	bool scene()
	{
		return state == SCENE;
	}

	bool is_grabbing()
	{
		return state != EMPTY;
	}

};
