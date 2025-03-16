#pragma once

namespace xpcube {

struct InputState {
    bool esc = false;
};

bool window_init();

void window_new_frame();
void window_end_of_frame();

InputState window_get_input();

void window_set_close();
bool window_should_close();

}