#include "platform.h"
#include "collision/collider.h"
#include "collision/collision_space.h"

#include <raylib.h>

namespace entities {

// Platrform functions 
//////////////////////////////////////////
platform_t platform_create(Vector2 pos, Vector2 size, Color color) { 
  return platform_t {
    .position = pos, 
    .size = size, 
    .color = color,
    .collider = collision::collider_add(pos, size, false),
  };
}

void platform_render(platform_t& platform) {
  DrawRectangleV(platform.position, platform.size, platform.color);
}
//////////////////////////////////////////

}
