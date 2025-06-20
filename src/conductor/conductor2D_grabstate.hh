#pragma once

#include "window/auxevent.hh"


struct GrabStateConductor : InputState {
	/** Subsystems that can recieve and respond to input events. */
	enum SubSys {
		PUI,
		QUAD,
		SCENE,
		NONE,
	} subsystem = NONE;

	void clear()
	{
		last_response = {};
		subsystem = NONE;
	}

	void update(SubSys subsys, InputResponse response)
	{
		update_state(response);
		subsystem = subsys;

		// If no grab state exists, then we unset the subsystem target
		if(is_all_release())
			subsystem = NONE;
	}

	bool pui()
	{
		return subsystem == PUI ? true : false;
	}

	bool quad()
	{
		return subsystem == QUAD ? true : false;
	}

	bool scene()
	{
		return subsystem == SCENE ? true : false;
	}


	bool pui_is_grabbing_mouse()
	{
		return pui() && is_grabbing_mouse();
	}

	bool quad_is_grabbing_mouse()
	{
		return quad() && is_grabbing_mouse();
	}

};
