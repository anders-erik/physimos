#ifndef WINDOW_CURSORS_HPP
#define WINDOW_CURSORS_HPP


typedef enum class PCursor {
    None,
    Default,
    Point,
    Pan,
    Hori,
    Vert,
} PCursor;

void set_cursor(PCursor cursor);

#endif