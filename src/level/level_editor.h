#pragma once

#include "scenes/main_scene.h"

namespace level {

// Level editor functions
/////////////////////////////////////////////
bool editor_is_active();
void editor_set_active(const bool active);
void editor_update(scenes::main_t* main); 
void editor_render();
void editor_ui_render();
/////////////////////////////////////////////

}
