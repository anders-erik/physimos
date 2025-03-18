#pragma once

#include "math.hh"

namespace xprubik {


struct MouseState {
    bool    middle_down = false;
    PointD  middle_prev_pos;
    PointD  middle_delta_accum;

    float scroll_accumulator = 0.0f;

    PointD cursor_pos;
};

struct InputState {
    bool esc = false;

    bool p = false;

    bool f = false;

    bool up = false;
    bool left = false;
    bool down = false;
    bool right = false;

    float scroll_delta = 0.0f;

    // Copies the authoritative state during creation
    MouseState mouse;
};

bool window_init();

void window_new_frame();
void window_end_of_frame();

InputState window_get_input();

void window_set_close();
bool window_should_close();

}