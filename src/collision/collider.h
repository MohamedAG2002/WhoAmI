#pragma once

#include <raylib.h>

namespace collision {

// Collision data type 
////////////////////////////////////////////////////
struct collision_data_t {
  Vector2 normal; 
  float depth; 
  bool has_collision;
};
////////////////////////////////////////////////////

// Collider type 
////////////////////////////////////////////////////
struct collider_t {
  Vector2 position, size; 

  int vertex_count;
  Vector2 vertices[4];

  bool is_dynamic;
};
////////////////////////////////////////////////////

// Collider functions
////////////////////////////////////////////////////
collision_data_t test_collisions(const collider_t& coll1, const collider_t& coll2);
void collider_update_vertices(collider_t& coll);
////////////////////////////////////////////////////

}
