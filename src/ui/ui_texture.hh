#ifndef UI_TEXTURE_HH
#define UI_TEXTURE_HH

#include "ui/ui_globals.hh"

namespace UI {
    namespace texture {
        
        void new_texture(unsigned int& texture_to_init);

        unsigned int get_static_color_texture(UI::Colors color);
        void init_static_textures();

    }
}

#endif