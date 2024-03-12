#pragma once

#include "entities/player.h"
#include "entities/platform.h"
#include "entities/trigger.h"

#include "collision/collider.h"

#include <vector>

namespace collision {

// Trigger callback 
////////////////////////////////////////////////////////////////////////////////////////////////
typedef void(*pfn_trigger_callback)(const entities::trigger_t&);
////////////////////////////////////////////////////////////////////////////////////////////////

// Collision space functions
////////////////////////////////////////////////////////////////////////////////////////////////
collider_t collider_add(Vector2 position, Vector2 size, bool dynamic);
void add_trigger_callback(const pfn_trigger_callback callback);
void check(entities::player_t& player, std::vector<entities::platform_t>& platforms, std::vector<entities::trigger_t>& triggers);
void resolve(entities::player_t& player);
////////////////////////////////////////////////////////////////////////////////////////////////

}
