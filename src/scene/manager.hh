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
namespace ManagerScene {

size_t new_unique_id();

scene::Scene2D* try_find_scene(size_t id);

[[nodiscard]] scene::Scene2D* new_scene(scene::Scene2D& _scene);
[[nodiscard]] scene::Scene2D* new_scene(f2 framebuffer_size);
[[nodiscard]] scene::Scene2D* get_scene(size_t id);


};