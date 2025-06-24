
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "auxwin.hh"

#include "auxkey.hh"



Key 
get_key_from_glfw_key(int _glfw_key)
{
    Key key;

    switch (_glfw_key){

    case GLFW_KEY_ESCAPE:           key = Key::Esc;     break;

    case GLFW_KEY_LEFT_SHIFT:       key = Key::shift;   break;
    case GLFW_KEY_RIGHT_SHIFT:      key = Key::shift;   break;

    case GLFW_KEY_LEFT_CONTROL:     key = Key::ctrl;    break;
    case GLFW_KEY_RIGHT_CONTROL:    key = Key::ctrl;    break;
    case GLFW_KEY_CAPS_LOCK:        key = Key::ctrl;    break;

    case GLFW_KEY_LEFT_ALT:         key = Key::alt;     break;
    case GLFW_KEY_RIGHT_ALT:        key = Key::alt;     break;


    case GLFW_KEY_F1:               key = Key::F1;      break;
    case GLFW_KEY_F2:               key = Key::F2;      break;
    case GLFW_KEY_F3:               key = Key::F3;      break;
    case GLFW_KEY_F4:               key = Key::F4;      break;
    case GLFW_KEY_F5:               key = Key::F5;      break;
    case GLFW_KEY_F6:               key = Key::F6;      break;
    case GLFW_KEY_F7:               key = Key::F7;      break;
    case GLFW_KEY_F8:               key = Key::F8;      break;
    case GLFW_KEY_F9:               key = Key::F9;      break;
    case GLFW_KEY_F10:              key = Key::F10;     break;
    case GLFW_KEY_F11:              key = Key::F11;     break;
    case GLFW_KEY_F12:              key = Key::F12;     break;


    case GLFW_KEY_A:        key = Key::a;       break;
    case GLFW_KEY_B:        key = Key::b;       break;
    case GLFW_KEY_C:        key = Key::c;       break;
    case GLFW_KEY_D:        key = Key::d;       break;
    case GLFW_KEY_E:        key = Key::e;       break;
    case GLFW_KEY_F:        key = Key::f;       break;
    case GLFW_KEY_G:        key = Key::g;       break;
    case GLFW_KEY_H:        key = Key::h;       break;
    case GLFW_KEY_I:        key = Key::i;       break;
    case GLFW_KEY_J:        key = Key::j;       break;
    case GLFW_KEY_K:        key = Key::k;       break;
    case GLFW_KEY_L:        key = Key::l;       break;
    case GLFW_KEY_M:        key = Key::m;       break;
    case GLFW_KEY_N:        key = Key::n;       break;
    case GLFW_KEY_O:        key = Key::o;       break;
    case GLFW_KEY_P:        key = Key::p;       break;
    case GLFW_KEY_Q:        key = Key::q;       break;
    case GLFW_KEY_R:        key = Key::r;       break;
    case GLFW_KEY_S:        key = Key::s;       break;
    case GLFW_KEY_T:        key = Key::t;       break;
    case GLFW_KEY_U:        key = Key::u;       break;
    case GLFW_KEY_V:        key = Key::v;       break;
    case GLFW_KEY_W:        key = Key::w;       break;
    case GLFW_KEY_X:        key = Key::x;       break;
    case GLFW_KEY_Y:        key = Key::y;       break;
    case GLFW_KEY_Z:        key = Key::z;       break;
    
    default:
        break;
    }

    return key;

}

