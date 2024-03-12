#include "level_editor.h"
#include "levelizer.h"
#include "entities/platform.h"
#include "entities/trigger.h"

#include <raylib.h>

#include <vector>

namespace level {

// Level editor type
/////////////////////////////////////////////
struct level_editor_t {
  bool is_editing = false;

  entities::platform_t* curr_plat = nullptr;
  entities::trigger_t* curr_trig = nullptr;
};

static level_editor_t s_editor{};
/////////////////////////////////////////////

// Private functions 
/////////////////////////////////////////////
static void add_platform(std::vector<entities::platform_t>& platforms, const Camera2D& cam) {
  Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), cam);
  entities::platform_t plat = entities::platform_create(world_mouse, Vector2(32.0f, 32.0f), BLACK);
  platforms.push_back(plat);

  // Set the current selected platform as the recently added one 
  s_editor.curr_plat = &platforms[platforms.size() - 1];
}

static void edit_current_platform(const Camera2D& cam) {
  if(IsKeyDown(KEY_LEFT)) {
    s_editor.curr_plat->size.x -= IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  else if(IsKeyDown(KEY_RIGHT)) {
    s_editor.curr_plat->size.x += IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  
  if(IsKeyDown(KEY_DOWN)) {
    s_editor.curr_plat->size.y += IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  else if(IsKeyDown(KEY_UP)) {
    s_editor.curr_plat->size.y -= IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }

  Vector2 pos = s_editor.curr_plat->position;
  Vector2 size = s_editor.curr_plat->size;
  Rectangle rec = {pos.x, pos.y, size.x, size.y};
  
  Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), cam);
  if(CheckCollisionPointRec(world_mouse, rec) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    s_editor.curr_plat->position = Vector2(world_mouse.x - (size.x / 2.0f), world_mouse.y - (size.y / 2.0f));
  }

  s_editor.curr_plat->collider.position = s_editor.curr_plat->position;
  s_editor.curr_plat->collider.size = s_editor.curr_plat->size;
  collision::collider_update_vertices(s_editor.curr_plat->collider);
}

static void add_trigger(std::vector<entities::trigger_t>& trigger, int trig_type, const Camera2D& cam) {
  Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), cam);
  entities::trigger_t trig = entities::trigger_create(world_mouse, 
                                                      Vector2(64.0f, 64.0f), 
                                                      (entities::trigger_type_e)trig_type, 
                                                      true);
  trigger.push_back(trig);

  s_editor.curr_trig = &trigger[trigger.size() - 1];
}

static void edit_current_trigger(const Camera2D& cam) {
  if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT)) {
    s_editor.curr_trig->size.x -= IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_RIGHT)) {
    s_editor.curr_trig->size.x += IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  
  if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_DOWN)) {
    s_editor.curr_trig->size.y += IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }
  else if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_UP)) {
    s_editor.curr_trig->size.y -= IsKeyDown(KEY_LEFT_SHIFT) ? 10 : 1;
  }

  Vector2 pos = s_editor.curr_trig->position;
  Vector2 size = s_editor.curr_trig->size;
  Rectangle rec = {pos.x, pos.y, size.x, size.y};

  Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), cam);
  if(CheckCollisionPointRec(world_mouse, rec) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    s_editor.curr_trig->position = Vector2(world_mouse.x - size.x / 2.0f, world_mouse.y - size.y / 2.0f);
  }
  
  s_editor.curr_trig->collider.position = s_editor.curr_trig->position;
  collision::collider_update_vertices(s_editor.curr_trig->collider);
}
/////////////////////////////////////////////

// Level editor functions
/////////////////////////////////////////////
bool editor_is_active() {
  return s_editor.is_editing;
}

void editor_set_active(const bool active) {
  s_editor.is_editing = active;
}

void editor_update(scenes::main_t* main) {
  if(!s_editor.is_editing) {
    return;
  } 

  // Select entities 
  if(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    bool can_exit = false;
    
    for(auto& plat : main->platforms) {
      Vector2 pos = plat.position;
      Vector2 size = plat.size;
      Rectangle rec = {pos.x, pos.y, size.x, size.y};
      Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), main->cam);

      if(CheckCollisionPointRec(world_mouse, rec)) {
        s_editor.curr_plat = &plat;
        can_exit = true;
        break;
      }
    }

    if(can_exit) {
      return;
    }

    for(auto& trig : main->triggers) {
      Vector2 pos = trig.position;
      Vector2 size = trig.size;
      Rectangle rec = {pos.x, pos.y, size.x, size.y};
      Vector2 world_mouse = GetScreenToWorld2D(GetMousePosition(), main->cam);

      if(CheckCollisionPointRec(world_mouse, rec)) {
        s_editor.curr_trig = &trig;
        can_exit = true;
        break;
      }
    }
  }

  // Add platform
  if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_L)) {
    add_platform(main->platforms, main->cam);
  }

  // Edit platform
  if(s_editor.curr_plat) {
    edit_current_platform(main->cam); 
  }
  
  // Edit trigger 
  if(s_editor.curr_trig) {
    edit_current_trigger(main->cam);
  }

  // Add trigger 
  bool can_add_trig = IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_T);
  int trig_type = 0;
  if(can_add_trig) {
    if(IsKeyPressed(KEY_ZERO)) {
      trig_type = 0;
    }
    else if(IsKeyPressed(KEY_ONE)) {
      trig_type = 1;
    }
    else if(IsKeyPressed(KEY_TWO)) {
      trig_type = 2;
    }
    else if(IsKeyPressed(KEY_THREE)) {
      trig_type = 3;
    }
    else if(IsKeyPressed(KEY_FOUR)) {
      trig_type = 4;
    }
    else if(IsKeyPressed(KEY_FIVE)) {
      trig_type = 5;
    }
    else if(IsKeyPressed(KEY_KP_6)) {
      trig_type = 6;
    }
    else {
      return;
    }

    add_trigger(main->triggers, trig_type, main->cam);
  }

  // Save the changes 
  if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
    printf("[EDITOR]: SAVED!\n");
    levelizer_save_player(main->player);
    levelizer_save_platforms(main->platforms);
    levelizer_save_triggers(main->triggers);
  }
}

void editor_render() {
  if(s_editor.curr_plat) {
    Vector2 pos = s_editor.curr_plat->position;
    Vector2 size = s_editor.curr_plat->size;

    DrawRectangleLines(pos.x, pos.y, size.x, size.y, YELLOW);
  }
  
  if(s_editor.curr_trig) {
    Vector2 pos = s_editor.curr_trig->position;
    Vector2 size = s_editor.curr_trig->size;

    DrawRectangleLines(pos.x, pos.y, size.x, size.y, YELLOW);
  }
}

void editor_ui_render() {
  if(!s_editor.is_editing) {
    return;
  }

  DrawText("[ARROW KEYS] Edit sizes", 10, GetScreenHeight() - 175, 15, YELLOW);
  DrawText("[LEFT MOUSE BUTTON] Select entity", 10, GetScreenHeight() - 150, 15, YELLOW);
  DrawText("[LEFT MOUSE BUTTON + HOLD] Move entity", 10, GetScreenHeight() - 125, 15, YELLOW);
  DrawText("[RIGHT MOUSE BUTTON] Select entity", 10, GetScreenHeight() - 100, 15, YELLOW);
  DrawText("[SHIFT + L] Add platform", 10, GetScreenHeight() - 75, 15, YELLOW);
  DrawText("[SHIFT + T + (0 - 6)] Add trigger", 10, GetScreenHeight() - 50, 15, YELLOW);
  DrawText("[CTRL + F] Save progress", 10, GetScreenHeight() - 25, 15, YELLOW);
}
/////////////////////////////////////////////

}
