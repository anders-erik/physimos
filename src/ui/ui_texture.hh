#ifndef UI_TEXTURE_HH
#define UI_TEXTURE_HH

#include "ui/ui_globals.hh"

namespace UI {
    namespace texture {

        /** Allocates a new texture of size 1x1 with the color pink */
        void new_texture(unsigned int& texture_to_init);

        /** Loads all ui color textures into vram */
        void init_static_color_textures();
        /** Loads all ui icon textures from filesystem into vram */
        void init_static_icon_textures();

        unsigned int get_static_color_texture(UI::Colors color);
        unsigned int get_icon(UI::Icon icon);
    }
}

#endif