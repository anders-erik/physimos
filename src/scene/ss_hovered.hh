#pragma once

#include "scene/tago.hh"


namespace SS 
{

struct Hovered
{
    TagO tag;

    void set(TagO new_tag_click)    { tag = new_tag_click;  }
    void clear()                    { tag = TagO();         }

};


}

