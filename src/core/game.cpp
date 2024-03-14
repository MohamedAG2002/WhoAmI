#include "game.h"
#include "scenes/main_scene.h"
#include "managers/audio_manager.h"

#include <raylib.h>

namespace game {

struct game_t {
  scenes::main_t* main_scene; 
};

static game_t s_game;

static void update() {
  audio::update();
  scenes::main_update(s_game.main_scene);
}

static void render() {
  BeginDrawing();
  ClearBackground(s_game.main_scene->background_color);

  scenes::main_render(s_game.main_scene);

  DrawFPS(10, 10);
  EndDrawing();
}

void create(const char* title, int width, int height) {
  // Raylib init
  InitWindow(width, height, title);
  SetTargetFPS(60);
  ToggleFullscreen();

  // Systems init 
  audio::init();

  // Main scene init 
  s_game.main_scene = scenes::main_create();
}

void destroy() {
  scenes::main_destroy(s_game.main_scene);
  audio::shutdown();

  CloseWindow();
}

void run() {
  while(!WindowShouldClose()) {
    update();
    render();
  }
}

}
