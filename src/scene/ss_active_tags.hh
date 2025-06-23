#pragma once

#include "scene/tago.hh"


namespace SS 
{

struct ActiveTags
{
    enum Type {
        none,
        selected,
        quad_capture,
    } type = none;

    TagO tag_click;
    TagO tag_hover;

    TagO get_selected()      { return type == selected      ? tag_click : TagO();    }
    TagO get_quad_capture()  { return type == quad_capture  ? tag_click : TagO();    }

    void select(TagO new_tag_click)         { type = selected;      tag_click = new_tag_click;  }
    void unselect()                         { if( type == selected) type = none; tag_click = TagO();  }

    void hover_set(TagO new_hover_tag)  {   tag_hover = new_hover_tag;  }
    void capture_current_hover()            { type = quad_capture;  tag_click = tag_hover;      }
    void select_current_hover()             { type = selected;      tag_click = tag_hover;     }
    void release_quad()                     { type = selected;                                  }
    void hover_clear()                  {   tag_hover = TagO();         }
    

    bool has_hover() const                {  return tag_hover.oid != 0; }


    bool is_hovering_a_quad()
    {
        return has_hover() && tag_hover.is_quad();
    }

    bool is_hovering_captured()
    {
        return is_quad_capture() && tag_hover == tag_click;
    }
    
    bool is_selected()                      const
    {   
        return type == selected;
    }
    bool is_selected_not_capture()                      const
    {   
        return is_selected() &&  !is_quad_capture();
    }
    bool is_quad_capture()                  const
    {
        return type == quad_capture;
    }
    bool is_empty()                         const
    {
        return type == none;
    }
    bool is_active(TagO tag_check)          const
    {
        return tag_check == tag_click || tag_check == tag_hover;
    }
    
    bool is_selected(TagO tag_check)        const
    {
        return is_selected() && tag_check == tag_click;
    }

    bool is_quad_capture(TagO tag_check)    const
    {
        return is_quad_capture() && tag_check == tag_click;
    }
};


}

