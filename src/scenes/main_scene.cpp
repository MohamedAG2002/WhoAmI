#include "main_scene.h"
#include "entities/player.h"
#include "entities/platform.h"
#include "entities/trigger.h"
#include "managers/audio_manager.h"
#include "ui/text.h"
#include "collision/collision_space.h"
#include "managers/dialogue_manager.h"
#include "level/level_editor.h"
#include "level/levelizer.h"

#include <raymath.h>
#include <raylib.h>

#include <vector>

namespace scenes {

// Globals
////////////////////////////////////
static bool s_can_reset = false;
static int s_total_colors = 0;
static Color s_colors[] = {DARKGRAY, SKYBLUE, DARKBLUE, 
                           DARKGRAY, Color{30, 30, 30, 255}, RED, 
                           ORANGE, DARKBLUE, Color{30, 30, 30, 255}, 
                           DARKGRAY, ORANGE, DARKGRAY};
static Color s_curr_color = s_colors[0];
////////////////////////////////////

// Private enum to determine which text to address. 
// Why not use a map? Because fuck you that's you. I'm sorry.
// I'm kidding. I love you. No homo.
////////////////////////////////////
enum text_type_e {
  TT_MENU_TITLE  = 0, 
  TT_PLAY_TEXT   = 1, 
  TT_PAUSE_TITLE = 2, 
  TT_RESUME_TEXT = 3, 
  TT_ESCAPE_TEXT = 4,
};
////////////////////////////////////

// Private functions
////////////////////////////////////
static void menu_screen(main_t* main) {
  ui::text_fade_in(main->texts[TT_MENU_TITLE]);
  ui::text_render(main->texts[TT_MENU_TITLE]);
  
  ui::text_fade_in(main->texts[TT_PLAY_TEXT]);
  ui::text_render(main->texts[TT_PLAY_TEXT]);
}

static void pause_screen(main_t* main) {
  ui::text_render(main->texts[TT_PAUSE_TITLE]);
  ui::text_render(main->texts[TT_RESUME_TEXT]);
  ui::text_render(main->texts[TT_ESCAPE_TEXT]);
}
////////////////////////////////////

// Callbacks
////////////////////////////////////
void level_reset_callback(const entities::trigger_t& trigger) {
  if(trigger.type != entities::TRIGG_TYPE_LVL_RESET) {
    return;
  }

  s_can_reset = true;
}

void color_lerp_callback(const entities::trigger_t& trigger) {
  if(trigger.type != entities::TRIGG_TYPE_COLOR_LERP) {
    return;
  }

  s_curr_color = s_colors[++s_total_colors];
}
////////////////////////////////////

// Main scene functions
////////////////////////////////////
main_t* main_create() {
  // Create the dialogue "system" 
  dialogue::create("assets/dialogue/dialogue.txt"); 

  // Create the levelizer to load .lvl files 
  level::levelizer_create("assets/levels/main.lvl");

  // HEAP ALLOCATION HO HO HO HO HO AH AH AH
  main_t* main = new main_t{};

  // Player init 
  level::levelizer_load_player(main->player); 

  // Platforms init 
  level::levelizer_load_platforms(main->platforms);

  // Triggers init 
  level::levelizer_load_triggers(main->triggers);

  // Camera init 
  main->cam = Camera2D{
    .offset = Vector2{GetScreenWidth() / 2.0f - 16.0f, main->player.position.y - 32.0f}, 
    .target = main->player.position,
    .rotation = 0.0f, 
    .zoom = 1.0f,
  }; 

  // Other game variables init 
  main->is_paused = false; 
  main->has_started = false; 
  main->background_color = s_curr_color;

  Vector2 screen_size = Vector2{GetScreenWidth() / 1.0f, GetScreenHeight() / 1.0f};
  Vector2 center_screen = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}; 

  // Menu texts init 
  main->texts.push_back(ui::text_create("Who Am I?", 50, center_screen, BLACK, 0)); 
  main->texts.push_back(ui::text_create("[SPACE] PLAY", 30, Vector2{center_screen.x, center_screen.y + 50.0f}, Color{0, 0, 0, 0}, 0)); 

  // Pause texts init
  main->texts.push_back(ui::text_create("PAUSED", 50, center_screen, GREEN, 255)); 
  main->texts.push_back(ui::text_create("[P] Resume", 30, Vector2{center_screen.x, center_screen.y + 50.0f}, GREEN, 255)); 
  main->texts.push_back(ui::text_create("[ESC] Exit", 30, Vector2{center_screen.x, center_screen.y + 100.0f}, GREEN, 255)); 

  collision::add_trigger_callback(level_reset_callback);
  collision::add_trigger_callback(color_lerp_callback);

  return main;
}

void main_destroy(main_t* main) {
  main->texts.clear();
  dialogue::destroy();
  
  delete main;
}

void main_update(main_t* main) {
  if(!main->has_started) {

    // Start the game when SPACE is pressed
    if(IsKeyPressed(KEY_SPACE)) {
      main->has_started = true;
    }

    return; 
  }
 
  // Pausing/unpausing by pressing P
  if(IsKeyPressed(KEY_P)) {
    main->is_paused = !main->is_paused;
  }

  // Don't update anything while paused
  if(main->is_paused) {
    return; 
  }

  // Update the background color in case of a change 
  main->background_color.r = Lerp(main->background_color.r, s_curr_color.r, GetFrameTime());
  main->background_color.g = Lerp(main->background_color.g, s_curr_color.g, GetFrameTime());
  main->background_color.b = Lerp(main->background_color.b, s_curr_color.b, GetFrameTime());
  main->background_color.a = s_curr_color.a;
  
  // Reset the scene when pros_curr_colormpted 
  if((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) || s_can_reset) {
    main_reset(main);
    return;
  }

  // @NOTE: Debug only. Enabling/disabling the editor
  //if(IsKeyPressed(KEY_G)) {
    //level::editor_set_active(!level::editor_is_active());
  //}
  
  // @NOTE: Debug only. Editor update
  //level::editor_update(main);

  // Collision checking and resolution only when not editing 
  //if(!level::editor_is_active()) {
  collision::check(main->player, main->platforms, main->triggers);
  collision::resolve(main->player);
  //}
  
  entities::player_update(main->player);
  main->cam.target = main->player.position;
}

void main_render(main_t* main) {
  // Render the menu scene
  ////////////////////////////////////
  if(!main->has_started) {
    menu_screen(main);
  }
  ////////////////////////////////////

  // Render the pause scene
  ////////////////////////////////////
  if(main->is_paused) {
    pause_screen(main);
  }
  ////////////////////////////////////

  BeginMode2D(main->cam);
  // Render the player
  ////////////////////////////////////
  entities::player_render(main->player);
  ////////////////////////////////////

  // Render the platforms
  ////////////////////////////////////
  for(auto& plat : main->platforms) {
    entities::platform_render(plat);
  }
  ////////////////////////////////////

  // @NOTE: For debug builds only. Render the triggers 
  ////////////////////////////////////
  //for(auto& trig : main->triggers) {
    //entities::trigger_debug_render(trig);
  //}
  ////////////////////////////////////

  // Render the dialogue texts
  ////////////////////////////////////
  dialogue::render();
  ////////////////////////////////////
  
  // @NOTE: Debug only. Render the editor
  ////////////////////////////////////
  //level::editor_render();
  ////////////////////////////////////

  EndMode2D();
  
  // @NOTE: Debug only. Render the editor's UI
  ////////////////////////////////////
  //level::editor_ui_render();
  ////////////////////////////////////
}

void main_reset(main_t* main) {
  // Player reset 
  level::levelizer_load_player(main->player);

  // Camera reset
  main->cam = Camera2D{
    .offset = Vector2{GetScreenWidth() / 2.0f - 16.0f, main->player.position.y - 32.0f}, 
    .target = main->player.position,
    .rotation = 0.0f, 
    .zoom = 1.0f,
  }; 
 
  // Platforms reset
  main->platforms.clear();
  level::levelizer_load_platforms(main->platforms);

  // Triggers reset
  main->triggers.clear();
  level::levelizer_load_triggers(main->triggers);

  // Dialogue reset
  dialogue::reset();

  // Audio manager reset 
  audio::reset();

  // Variables reset
  s_can_reset = false;
  s_total_colors = 0;
  s_curr_color = s_colors[s_total_colors];
  main->background_color = s_curr_color;
  main->is_paused = false; 
  main->has_started = false; 
}
////////////////////////////////////

}
