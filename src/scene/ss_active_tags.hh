#pragma once

#include "scene/tago.hh"


namespace SS 
{

struct ActiveTags
{
    enum Type {
        none,
        selected,
    } type = none;

    TagO tag_selected;
    TagO tag_hover;

    TagO get_selected()      { return type == selected      ? tag_selected : TagO();    }

    void select(TagO new_tag_click)         { type = selected;      tag_selected = new_tag_click;  }
    void unselect()                         { type = none; tag_selected = TagO();  }

    void hover_set(TagO new_hover_tag)  {   tag_hover = new_hover_tag;  }
    void hover_clear()                  {   tag_hover = TagO();         }
    
    bool has_hover() const                {  return tag_hover.oid != 0; }
    bool has_selected() const           {   return type == selected; }
    /** Hovered or selected */
    bool is_active(TagO tag_check)  const   {   return tag_check == tag_selected || tag_check == tag_hover; }
    
    bool is_selected(TagO tag_check)    const {   return has_selected() && tag_check == tag_selected; }
};


}

