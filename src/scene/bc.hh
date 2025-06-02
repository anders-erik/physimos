#pragma once


#include <vector>

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
    Tag(size_t id, Type type, std::string label) : id {id}, type {type}, label{label} {};
};



/** Returns new scene, and immediately borrows the scene. */
[[nodiscard]] Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene(std::string label);

[[nodiscard]] OptPtr<scene::Scene2D> borrow_scene(Tag tag);

void return_scene(Tag tag);




};