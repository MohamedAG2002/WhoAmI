#include "trigger.h"
#include "collision/collider.h"
#include "collision/collision_space.h"

#include <raylib.h>

namespace entities {

// Trigger functions
/////////////////////////////////////////////////
trigger_t trigger_create(Vector2 pos, Vector2 size, trigger_type_e type, bool active) {
  return trigger_t {
    .position = pos, 
    .size = size, 
    .collider = collision::collider_add(pos, size, false), 
    .type = type, 
    .is_active = active,
  };
}

void trigger_debug_render(trigger_t& trigger) {
  Color color = WHITE;

  if(!trigger.is_active) {
    color = PURPLE;
  }

  DrawRectangleLines(trigger.position.x, trigger.position.y, trigger.size.x, trigger.size.y, color);  
}
/////////////////////////////////////////////////

}
