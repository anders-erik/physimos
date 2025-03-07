#ifndef UI_SHADER_STRING_HH
#define UI_SHADER_STRING_HH

#include <string>

#include "ui/ui_globals.hh"

#include "image/bitmap.hh"

namespace UI {
    namespace shader {

        typedef struct RenderString {
            std::string str;

            unsigned int vao;
            unsigned int vbo;

            unsigned int vertex_count;

            float primitiveTransform[16];
            
        } RenderString;

        typedef struct VertexTexture {
            float vx;
            float vy;
            float vz;
            float tx;
            float ty;
        } VertexTexture;

        
        typedef class StringShader {
            public:
                std::string vert_path = "src/ui/shaders/string_vert.glsl";
                std::string frag_path = "src/ui/shaders/string_frag.glsl";

                unsigned int shader_id;
                unsigned int vao;
                unsigned int vbo;

                unsigned int vertex_count;

                unsigned int uiViewportTransformLoc;
                unsigned int uiPrimitiveTransformLoc;
                unsigned int texture;

                /** Tranform matrix based on the current viewport dimensions */
                float viewportTransform16[16];
                

                /** Compiles, load locations, and set permanent vao. */
                void init();


                /** Set values needed for render draw. UI transform and texture.  */
                void set_primitive_transform(float* primitiveTransform_mat);
                void set_bitmap(pimage::Bitmap& bitmap);
                void set_texture(unsigned int new_texture);
                void set_vertex_data(VertexTexture* vertex_data, unsigned int sizeof_vertex_data);

                /** Makes rendering call with values previously specified using a set()-call. */
                void draw();

                /** Update the viewport transformation matrix using physimos window info */
                void set_window_info(float width, float height, float _xscale, float _yscale);

                StringShader();
        } StringShader;

        /** UI shader rendering a rectangle with texture. */
        extern StringShader string_shader;
    }
}

#endif