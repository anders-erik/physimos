#ifndef INPUT_HPP
#define INPUT_HPP

#include "Windowing.hpp"
#include "window_cursors.hh"


struct GLFWwindow;


namespace PInput {

/** Convenience enum to idnetify modifier key state. */
typedef enum class KeyMod {
	NONE,
	CTRL,
	CTRL_SHIFT,
	CTRL_SHIFT_ALT,
	CTRL_ALT,
	SHIFT,
	SHIFT_ALT,
	ALT,
} KeyMod;


/** Store key direction state: Up or Down */
typedef enum class KeyDir {
	Up 	= 1,
	Down = 2,
} KeyDir;

/** Physimos Key values */
typedef enum class PKey {

	// ASCII
	B = 66,
	Z = 91,
	
	// non-ascii chars
	Shift = 128,
	Ctrl = 129,
	Alt = 130,
	PageUp = 140,
	PageDown = 141,

	Unknown = 255,

} Pkey;

typedef struct KeyEvent {
	PKey pkey;
	KeyMod modifier;
	KeyDir direction;
} KeyEvent;


/** Get initial window info, start subscriptions */
void init();

// SUBSCRIBERS

/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_pointer_position_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos, ViewportCursor _pointer_change));

/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_left_down_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));
/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_mouse_left_release_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));

/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_middle_down_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));
/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_mouse_middle_release_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));
/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_scroll_y_conductor(void (*subscriberCallback)(double y_change));

/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_backward_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));
/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_mouse_forward_conductor(void (*subscriberCallback)(ViewportCursor _pointer_pos));

/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_key_up_conductor(void (*subscriberCallback)(KeyEvent key_event));
/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_key_down_conductor(void (*subscriberCallback)(KeyEvent key_event));



// GLFW CALLBACKS

/** GLFW will call on mouse button action */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
/** GLFW will call on any mouse position change */
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
/** Keystroke callback from windowing.
 * 	mods: 1=shift, 2=ctrl, 4=alt
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
/** GLFW will call on mouse scroll detection */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





/** LEGACY. Physimos input state structure */
struct InputState {
	int startSimClick = 0;
	// char s = 0;
	int pointerX = 0;
	int pointerY = 0;
	int mousePressActive = 0;
	int pointerXLastClick = 0;
	int pointerYLastClick = 0;
	int pointerXLastFrame = 0;
	int pointerYLastFrame = 0;
	// keys
	int middleMouse = 0;
	int ctrl = 0;
	int w = 0;
	int a = 0;
	int s = 0;
	int d = 0;
	char mostRecentADpress = 's';
	int au = 0;
	int al = 0;
	int ad = 0;
	int ar = 0;

};

/** LEGACY. Current input state. */
extern InputState inputState;

}







#endif