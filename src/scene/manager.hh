#pragma once


#include <vector>

#include "math/vecmat.hh"

#include "log.hh"
#include "print.hh"
#include "opt.hh"
#include "pair.hh"



// Forward declares
namespace scene {
    struct Scene2D;
}



/** Scene Owner and manager module. */
namespace ManagerScene {

size_t new_unique_id();

scene::Scene2D* try_find_scene(size_t id);

/** Initializes manager and creates the root scene. The root scene is returned. */
scene::Scene2D* init(f2 window_size);


scene::Scene2D* new_scene(scene::Scene2D& _scene);
scene::Scene2D* new_scene(f2 framebuffer_size);

[[nodiscard]] scene::Scene2D* get_root_scene();
[[nodiscard]] scene::Scene2D* get_scene(size_t id);

void update_current_target(f2 cursor_pos_window_normalized);
void clear_current_target();
scene::Scene2D* get_current_target();

};