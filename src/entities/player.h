#pragma once

#include "collision/collider.h"

#include <raylib.h>

namespace entities {

// Player type
////////////////////////////////////////
struct player_t {
  Vector2 position, velocity; 
  Color color;
  collision::collider_t collider;
  bool is_active, is_grounded;
};
////////////////////////////////////////

// Player functions
////////////////////////////////////////
player_t player_create(Vector2 initial_pos);
void player_update(player_t& player);
void player_render(player_t& player);
////////////////////////////////////////

}
