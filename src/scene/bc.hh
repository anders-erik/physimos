#pragma once


#include <vector>

#include "math/vecmat.hh"

#include "log.hh"
#include "print.hh"
#include "opt.hh"
#include "pair.hh"



// FORWARD DECLARES

namespace scene {

struct Scene2D;
struct SubScene2D;

}





// Runtime Borrow checker
namespace BC {

struct obj {
    std::vector<scene::SubScene2D> scenesss;
};

enum Type {
    none,
    scene,
};


struct Tag {
    unsigned int id;
    Type type;
    std::string label;

    Tag() = default;
    Tag(size_t id, Type type) : id {id}, type {type} {};
    Tag(size_t id, Type type, std::string label) : id {id}, type {type}, label{label} {};
};

static inline OptPtr<scene::Scene2D> try_find_scene(Tag tag);

/** Returns new scene, and immediately borrows the scene. */
[[nodiscard]] Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene_with_lock(std::string label);
[[nodiscard]] OptPtr<scene::Scene2D> borrow_scene(Tag tag); /** Applies borrow lock */
void return_scene(Tag tag); /** Lift borrow lock */


/** New scene without lock.  */
[[nodiscard]] Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene(scene::Scene2D& _scene);
/** New scene without lock.  */
[[nodiscard]] Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene(f2 framebuffer_size);
/** Get scene without lock.  */
[[nodiscard]] OptPtr<scene::Scene2D> get_scene(Tag tag);



};