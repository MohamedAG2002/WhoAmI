#include "collision_space.h"

#include "entities/player.h"
#include "entities/platform.h"

#include <vector>

namespace collision {

// Collision space type
////////////////////////////////////////////////////////////////////////////////////////////////
struct collision_space_t {
  std::vector<collider_t> colliders;
  std::vector<collision_data_t> collisions;
  std::vector<pfn_trigger_callback> trigger_callbacks;
};

static collision_space_t s_space;
////////////////////////////////////////////////////////////////////////////////////////////////

// Private functions
////////////////////////////////////////////////////////////////////////////////////////////////
void call_trigger_callbacks(const entities::trigger_t& trigger) {
  for(auto& fn : s_space.trigger_callbacks) {
    fn(trigger);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////

// Collision space functions
////////////////////////////////////////////////////////////////////////////////////////////////
collider_t collider_add(Vector2 position, Vector2 size, bool dynamic) {
  collider_t coll; 
 
  coll.position = position;
  coll.size = size; 
  coll.is_dynamic = dynamic;

  Vector2 min = Vector2(position.x, position.y);
  Vector2 max = Vector2(position.x + size.x, position.y + size.y);
  coll.vertex_count = 4;
  coll.vertices[0] = min;
  coll.vertices[1] = Vector2(min.x, max.y);
  coll.vertices[2] = Vector2(max.x, min.y);
  coll.vertices[3] = max;
  
  s_space.colliders.push_back(coll);
  return coll;
}

void add_trigger_callback(const pfn_trigger_callback callback) {
  s_space.trigger_callbacks.push_back(callback);
}

void check(entities::player_t& player, std::vector<entities::platform_t>& platforms, std::vector<entities::trigger_t>& triggers) {
  collider_update_vertices(player.collider); 

  for(auto& plat : platforms) {
    collision_data_t data = test_collisions(player.collider, plat.collider);
    if(data.has_collision) {
      s_space.collisions.push_back(data);
    }
  }
  
  for(auto& trig : triggers) {
    if(!trig.is_active) {
      continue;
    }

    collision_data_t data = test_collisions(player.collider, trig.collider);
    if(data.has_collision) {
      call_trigger_callbacks(trig);

      // Disable the trigger so that is cannot be collided with again. Done to avoid constant spam of the same trigger 
      trig.is_active = false;
    }
  }
}

void resolve(entities::player_t& player) {
  for(auto& coll : s_space.collisions) {
    player.position.x -= (coll.normal.x * (coll.depth + 0.7f));
    player.position.y -= (coll.normal.y * (coll.depth));
    player.velocity = Vector2(0.0f, 0.0f);

    if(coll.normal.y == 1) {
      player.is_grounded = true;
    }
  }

  s_space.collisions.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////

}
