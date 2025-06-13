
#include <iostream>

#include "box2D.hh"



Box2D Box2D::subbox_to_normalized(Box2D parent_box, Box2D child_box)
{
    // new
    // f2 pos_norm = child_box.pos / parent_box.size;
    // f2 size_norm = child_box.size / parent_box.size;
    // Box2D congruent_subbox;
    // congruent_subbox.pos = pos + pos_norm + size;
    // congruent_subbox.size = size * size_norm;
    // return congruent_subbox;

    float normalized_pos_x = child_box.pos.x / parent_box.size.x;
    float normalized_pos_y = child_box.pos.y / parent_box.size.y;

    float normalized_size_x = child_box.size.x / parent_box.size.x;
    float normalized_size_y = child_box.size.y / parent_box.size.y;

    f2 pos_norm  = {normalized_pos_x, normalized_pos_y};
    f2 size_norm = {normalized_size_x, normalized_size_y};


    Box2D prop_sub_box;

    prop_sub_box.pos.x = pos.x + pos_norm.x * size.x;
    prop_sub_box.pos.y = pos.y + pos_norm.y * size.y;

    prop_sub_box.size.x = size.x * size_norm.x;
    prop_sub_box.size.y = size.y * size_norm.y;

    return prop_sub_box;
}


f2 Box2D::to_normalized(f2 point)
{
    float x = ( point.x - pos.x ) / size.x;
    float y = ( point.y - pos.y ) / size.y;
    return f2(x, y);
}

f2 Box2D::to_box(f2 normalized)
{
    return f2 {
        pos.x + size.x * normalized.x, 
        pos.y + size.y * normalized.y
    };
}

f2 Box2D::to_box_delta(f2 normalized)
{
    return f2 {
        size.x * normalized.x, 
        size.y * normalized.y
    };
}

bool Box2D::contains_point(f2 p)
{
    bool x_pass = p.x > pos.x && p.x < pos.x + size.x;
    bool y_pass = p.y > pos.y && p.y < pos.y + size.y;

    return x_pass && y_pass ? true : false;
}

m3f3 Box2D::get_matrix_m3f3() const
{
    m3f3 M;

    M.x.z = pos.x;
    M.y.z = pos.y;

    M.x.x = size.x;
    M.y.y = size.y;

    return M;
}

void Box2D::print(){
    std::cout << "Box2D: " << std::endl;
    std::cout << "box.pos.x  = " << pos.x << std::endl;
    std::cout << "box.pos.y  = " << pos.y << std::endl;
    std::cout << "box.size.x = " << size.x << std::endl;
    std::cout << "box.size.y = " << size.y << std::endl;
    
}

Box2D Box2D::to_normalized_box(const Box2D & box, const Box2D & subbox)
{
    Box2D n_subbox;

    n_subbox.size = subbox.size / box.size;
    n_subbox.pos = (subbox.pos - box.pos) / box.size;

    return n_subbox;
}

Box2D Box2D::from_normalized_box(const Box2D & box, const Box2D & n_subbox)
{
    Box2D subbox;

    subbox.size = n_subbox.size * box.size;
    subbox.pos  = n_subbox.pos * box.size;
    subbox.pos  += box.pos;

    return subbox;
}
