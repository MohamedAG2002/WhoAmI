#pragma once

#include "collision/collider.h"

#include <raylib.h>

namespace entities {

// Platrform type 
//////////////////////////////////////////
struct platform_t {
  Vector2 position, size;
  Color color;
  collision::collider_t collider;
};
//////////////////////////////////////////

// Platrform functions 
//////////////////////////////////////////
platform_t platform_create(Vector2 pos, Vector2 size, Color color);
void platform_render(platform_t& platform);
//////////////////////////////////////////

}
