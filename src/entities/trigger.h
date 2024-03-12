#pragma once

#include "collision/collider.h"

#include <raylib.h>

namespace entities {

// Trigger type enum 
/////////////////////////////////////////////////
enum trigger_type_e {
  TRIGG_TYPE_TEXT        = 0, 
  TRIGG_TYPE_SOUND       = 1, 
  TRIGG_TYPE_MUSIC       = 2,
  TRIGG_TYPE_MUSIC_STOP  = 3,
  TRIGG_TYPE_JUMP_PAD    = 4,
  TRIGG_TYPE_LVL_RESET   = 5,
  TRIGG_TYPE_COLOR_LERP  = 6,
};
/////////////////////////////////////////////////

// Trigger type 
/////////////////////////////////////////////////
struct trigger_t {
  Vector2 position, size;

  collision::collider_t collider; 

  trigger_type_e type;
  bool is_active;
};
/////////////////////////////////////////////////

// Trigger functions
/////////////////////////////////////////////////
trigger_t trigger_create(Vector2 pos, Vector2 size, trigger_type_e type, bool active);
void trigger_debug_render(trigger_t& trigger);
/////////////////////////////////////////////////

}
