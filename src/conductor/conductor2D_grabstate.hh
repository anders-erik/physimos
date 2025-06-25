#pragma once

#include "window/auxevent.hh"



struct PhysimosGrab
{
	typedef unsigned short 	SubSystemBits;
	typedef unsigned short 	PhysimosGrabBits;

    static const SubSystemBits NONE    	= 0x0000;

    static const SubSystemBits PUI   	= 0x0100;
    static const SubSystemBits QUAD  	= 0x0200;
    static const SubSystemBits SCENE 	= 0x0400;

	static const PhysimosGrabBits PUI_MOUSE_GRAB   	= PUI 	| InputResponse::MOUSE_GRAB;
	static const PhysimosGrabBits QUAD_MOUSE_GRAB  	= QUAD	| InputResponse::MOUSE_GRAB;
	static const PhysimosGrabBits SCENE_MOUSE_GRAB 	= SCENE | InputResponse::MOUSE_GRAB;
	static const PhysimosGrabBits SCENE_MOUSE_PAN	= SCENE | InputResponse::MOUSE_PAN;


	PhysimosGrabBits state = 0x0000;


	void update(SubSystemBits new_subsystem, InputResponse new_response)
	{
		if(new_response.is_release_both())
		{
			state = NONE;
			return;
		}

		state  = new_subsystem;
		state |= new_response.action;
	}

	bool is_pui() 	{ return (state & PUI) 		== PUI; 	}
	bool is_quad() 	{ return (state & QUAD) 	== QUAD; 	}
	bool is_scene() { return (state & SCENE)	== SCENE; 	}

	bool is_pui_mouse_grab() 	{ return (state & PUI_MOUSE_GRAB)	== PUI_MOUSE_GRAB;		}
	bool is_quad_mouse_grab() 	{ return (state & QUAD_MOUSE_GRAB)	== QUAD_MOUSE_GRAB;		}
	bool is_scene_mouse_grab() 	{ return (state & SCENE_MOUSE_GRAB)	== SCENE_MOUSE_GRAB;	}
	bool is_scene_mouse_pan() 	{ return (state & SCENE_MOUSE_PAN)	== SCENE_MOUSE_PAN;		}


	bool is_non_empty()	{	return state != NONE;			}

};
