
#pragma once

#include <array>

#include "math/vecmat.hh"
#include "math/transform.hh"
#include "math/geometry/shape.hh"

#include "opengl/program.hh"


namespace opengl {


struct VertexShape2D {
    f3 pos;
    f2 tex;
};

struct ShapeS2DRenderContext {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    int element_count;

    void delete_contents();
};



struct ShapeS2D {
    m3f3 M_m_s;
    std::vector<VertexShape2D> verts;
    Shape shape;
    f2 text_coord; // Texture coordinate for coloring texture
    ShapeS2DRenderContext render_context;

    std::array<VertexShape2D, 6> verts_6;
    Transform2D transform_2d;



    ShapeS2D();
    ShapeS2D(Shape &shape);

    void create_point(f2 point);
    void create_line(f2 p1, f2 p2);
    void create_fan(std::vector<f2>& points);

    bool is_point();
    bool is_line();

    void set_texture(f2 text_coord);

    m3f3 get_matrix();


    void set_dims(float window_width, float window_height, float width_pixels, float height_pixels);

    static std::array<VertexShape2D, 6> generate_quad();

};


class Scene2DRenderer {
    opengl::Programs shader = opengl::Programs::Texture2D;
    unsigned int program; // opengl id for renderer

public:
    Scene2DRenderer();

    void create_context_quad(ShapeS2D& quad);
    void create_context(ShapeS2D& shape);

    void activate();
    void set_camera(m3f3 camera);
    void set_model(m3f3 model_mat);

    void render_quad(ShapeS2DRenderContext context);
    void render_point(ShapeS2DRenderContext context);
    void render_multisample_texture(ShapeS2DRenderContext context);
    
};


}