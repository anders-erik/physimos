#ifndef INPUT_HPP
#define INPUT_HPP

#include "Windowing.hpp"


struct GLFWwindow;


namespace PInput {


/** Current Pointer position and change since previous position. y axis is inverted from screen coordinates (y=0 is bottom edge) */
typedef struct PointerPosition {
	double x;
	double y;
} PointerPosition;
typedef struct PointerChange {
	double dx;
	double dy;
} PointerChange;

/** 
 * 	Convenience struct for later clarity of usage.
 * 	Extracted from GLFW mods: 1=shift, 2=ctrl, 4=alt 
*/
typedef struct ModifierState {
	bool ctrl;
	bool alt;
	bool shift;
} ModifierState;
// extern ModifierState modifier_state;

/** Store key direction state: Up or Down */
typedef enum class KeyDir {
	Up 	= 1,
	Down = 2,
} KeyDir;

/** Physimos Key values */
typedef enum class PKey {

	B = 10,
	
	PageUp = 100,
	PageDown = 101,

	Unknown = 255,

} Pkey;

typedef struct KeyEvent {
	PKey pkey;
	ModifierState modifier;
	KeyDir direction;
} KeyEvent;


/** Get initial window info, start subscriptions */
void init();

// SUBSCRIBERS

/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_pointer_position_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos, PointerChange _pointer_change));
/** Called by external module to subscribe to any change in the pointer position within physimos window. */
void subscribe_mouse_left_release_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos));
/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_left_down_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos));
/** Called by external module to subscribe to any left click detected within physimos window. */
void subscribe_mouse_scroll_y_conductor(void (*subscriberCallback)(double y_change));

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