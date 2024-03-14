#include "collider.h"

#include <raylib.h>
#include <raymath.h>

namespace collision {

// Private functions
////////////////////////////////////////////////////
Vector2 get_aabb_interval(const collider_t& aabb, Vector2 axis) {
  Vector2 result = {0.0f, 0.0f}; 
  result.x = Vector2DotProduct(aabb.vertices[0], axis);
  result.y = result.x;

  for(int i = 1; i < aabb.vertex_count; i++) {
    float proj = Vector2DotProduct(aabb.vertices[i], axis);

    if(proj < result.x) {
      result.x = proj;
    }
    if(proj > result.y) {
      result.y = proj;
    }
  }

  return result;
}

bool check_axis_overlap(collision_data_t& man, const collider_t& aabb1, const collider_t& aabb2, Vector2 axis) {
  Vector2 interval_a = get_aabb_interval(aabb1, axis);
  Vector2 interval_b = get_aabb_interval(aabb2, axis);

  // No overlap therefore no collision
  if((interval_a.x >= interval_b.y) || (interval_b.x >= interval_a.y)) {
    return false;
  }

  float depth = fminf(interval_b.y - interval_a.x, interval_a.y - interval_b.x);

  if(depth < man.depth) {
    man.depth = depth;
    man.normal = axis;
  }

  return true;
}

Vector2 get_arithmatic_mean(const int count, const Vector2* vertices) {
  float sum_x = 0.0f;
  float sum_y = 0.0f;

  for(int i = 0; i < count; i++) {
    sum_x += vertices[i].x;
    sum_y += vertices[i].y;
  }

  return Vector2{sum_x / (float)count, sum_y / (float)count};
}
////////////////////////////////////////////////////

// Collider functions
////////////////////////////////////////////////////
collision_data_t test_collisions(const collider_t& coll1, const collider_t& coll2) {
  collision_data_t data; 
  
  data.depth = std::numeric_limits<float>().max();
  data.normal = Vector2{0.0f, 0.0f};
  data.has_collision = true; 

  int axis_count = 2;
  Vector2 axis[] = {
    {0, 1}, 
    {1, 0},
  };

  for(int i = 0; i < axis_count; i++) {
    // If there is no collision on even axis, this means the bodies are not colliding
    if(!check_axis_overlap(data, coll1, coll2, axis[i])) {
      data.has_collision = false; 
      return data; 
    }
  }

  Vector2 center_a = get_arithmatic_mean(coll1.vertex_count, coll1.vertices);
  Vector2 center_b = get_arithmatic_mean(coll2.vertex_count, coll2.vertices);
  Vector2 dir = Vector2Subtract(center_b, center_a);

  if(Vector2DotProduct(dir, data.normal) < 0.0f) {
    data.normal = Vector2Negate(data.normal);
  }

  return data;
}

void collider_update_vertices(collider_t& coll) {
  Vector2 min = Vector2{coll.position.x, coll.position.y};
  Vector2 max = Vector2{coll.position.x + coll.size.x, coll.position.y + coll.size.y};
  coll.vertices[0] = min;
  coll.vertices[1] = Vector2{min.x, max.y};
  coll.vertices[2] = Vector2{max.x, min.y};
  coll.vertices[3] = max;
}
////////////////////////////////////////////////////

}
