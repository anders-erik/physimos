#pragma once

#include <functional>

struct Auxwin;



using HandleFramebufferSizeFn = std::function<void(int, int)>;
// using HandleFramebufferSizeFn =  void (*)(int, int);
using HandleMouseButtonFn = void (*)(int, int, int);
using HandleCursorPositionFn = void (*)(double, double);
using HandleScrollFn = void (*)(double, double);
using HandleKeyFn = void (*)(int, int, int);

struct AuxwinCallbacks
{
    HandleFramebufferSizeFn     framebuffer_size    = nullptr;
    HandleMouseButtonFn         mouse_button        = nullptr;
    HandleCursorPositionFn      cursor_position     = nullptr;
    HandleScrollFn              scroll              = nullptr;
    HandleKeyFn                 key                 = nullptr;
};


AuxwinCallbacks get_auxwin_callbacks();
void set_auxwin_callbacks(AuxwinCallbacks callbacks);