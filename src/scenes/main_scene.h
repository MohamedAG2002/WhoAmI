#pragma once

#include "entities/player.h"
#include "entities/platform.h"
#include "entities/trigger.h"

#include "ui/text.h"

#include <raylib.h>

#include <vector>

namespace scenes {

// Main scene type
////////////////////////////////////
struct main_t {
  Camera2D cam;

  entities::player_t player;
  std::vector<entities::platform_t> platforms;
  std::vector<entities::trigger_t> triggers;

  bool is_paused, has_started;
  Color background_color; 

  std::vector<ui::text_t> texts;
};
////////////////////////////////////

// Main scene functions
////////////////////////////////////
main_t* main_create(); 
void main_destroy(main_t* main);
void main_update(main_t* main);
void main_render(main_t* main);
void main_reset(main_t* main);
////////////////////////////////////

}
