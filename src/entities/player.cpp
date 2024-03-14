#include "player.h"
#include "entities/trigger.h"
#include "collision/collider.h"
#include "collision/collision_space.h"

#include <raylib.h>

namespace entities {

// Consts
////////////////////////////////////////
const float PLAYER_MOVE_SPEED = 250.0f;
const float PLAYER_JUMP_SPEED = 250.0f;
const float GRAVITY = 9.81f;
////////////////////////////////////////

// GLOBALS??!!! YUCK!!!!!
////////////////////////////////////////
static int grav_mul = 1; // Can inverse the gravity when needed
////////////////////////////////////////

// Private functions
////////////////////////////////////////
static void move_player(player_t& player) {
  if(IsKeyDown(KEY_A)) {
    player.velocity.x = -PLAYER_MOVE_SPEED;
  }
  else if(IsKeyDown(KEY_D)) {
    player.velocity.x = PLAYER_MOVE_SPEED;
  }
  else {
    player.velocity.x = 0.0f;
  }

  if(IsKeyPressed(KEY_SPACE) && player.is_grounded) {
    player.velocity.y = -PLAYER_JUMP_SPEED;
    player.is_grounded = false;
  }
  else {
    player.velocity.y += (GRAVITY * grav_mul);
  }
}
////////////////////////////////////////

// Callbacks 
////////////////////////////////////////
void inverse_gravity_callback(const trigger_t& trigger) {
  if(trigger.type != TRIGG_TYPE_JUMP_PAD) {
    return;
  }

  grav_mul = grav_mul == -1 ? 1 : -1;
}
////////////////////////////////////////

// Player functions
////////////////////////////////////////
player_t player_create(Vector2 initial_pos) {
  player_t player =  {
    .position = initial_pos, 
    .velocity = Vector2{0.0f, 0.0f},
    .color = GREEN,
    .collider = collision::collider_add(initial_pos, Vector2{32.0f, 64.0f}, true),
    .is_active = true,
    .is_grounded = false,
  };

  collision::add_trigger_callback(inverse_gravity_callback);
  return player;
}

void player_update(player_t& player) {
  if(!player.is_active) {
    return;
  }

  move_player(player);
  player.position.x += player.velocity.x * GetFrameTime();
  player.position.y += player.velocity.y * GetFrameTime();
  
  // Update the position of the collider 
  player.collider.position.x = player.position.x;
  player.collider.position.y = player.position.y; 
}

void player_render(player_t& player) {
  DrawRectangleV(player.position, Vector2{32.0f, 64.0f}, player.color);
}
////////////////////////////////////////

}
