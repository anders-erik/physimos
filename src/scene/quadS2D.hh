
#pragma once

#include "lib/str.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"

// Forward declares
namespace {
    struct Bitmap;}
namespace opengl {
    struct Texture;}
namespace scene {
    struct Scene2D;}


namespace scene {



/**
    Math Box2D wrapper with a quad id, name, and texture id.  
    Currently implemented as an AABB.
*/
class QuadS2D 
{

public:

    enum class T {
        Bitmap,
        Scene2D,
    };

private:


    T type = T::Bitmap;
    Str name; // given name

    size_t object_id = 0; // the id of the texture provider (Scene, texture_id, bitmap, etc.)

    Box2D box;  // Composition instead of inheritence
    m3f3 M_m_s; // Ownership of queried matrix for safe use of references

    unsigned int texture_id; // opengl id : to be owned by subscene, canvas, etc.

public:

    size_t id; // index of 0 = no quad exists for this id
    // QuadS2D(size_t id): id {id} {};
    
    QuadS2D() = default;

    /** Quad id. Read only. */
    const size_t get_id() const;

    void set_name(Str new_name);
    const Str& get_name() const;

    void set_box(f2 pos, f2 size);
    Box2D get_box();
    m3f3 get_model_matrix() const;
    f2& get_pos_mut();
    f2& get_size_mut();

    void set_texture_id(unsigned int new_id);
    unsigned int get_texture_id() const;

    size_t get_object_id();

    void set_bitmap(Bitmap& texture_object);
    void set_bitmap(opengl::Texture& texture_object);
    /** Bitmap object will eventually provide a texture id itself */
    void set_bitmap_texture(unsigned int texture_id);
    void set_scene(const Scene2D* new_scene);
    bool is_bitmap();
    bool is_scene2D();
    /** Dynamic texture objects (such as scenes) will be re-rendered. */
    void update_texture();


    // Compares provided point to Box2D bounds [scene coordinates]
    bool contains_cursor(f2 cursor_pos_scene_coords);
    // Converts provided scene-point to local quad-coordinates ( [0,1]x[0,1] )
    f2 scene_to_quad_normalized(f2 cursor_pos_scene_coords);

};



}