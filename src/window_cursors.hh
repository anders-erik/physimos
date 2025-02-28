#ifndef WINDOW_CURSORS_HPP
#define WINDOW_CURSORS_HPP

/** Rendered Cursors available.  */
typedef enum class PCursor {
    None,
    Default,
    Point,
    Pan,
    Hori,
    Vert,
} PCursor;


/** Represents a cursor in the viewport, always expressed in logical pixel units. 
 *  Increasing y is up. Increasing x is to the right. 
 */
typedef struct ViewportCursor {
    double x;
    double y;
} ViewportCursor;

void set_cursor(PCursor cursor);

#endif