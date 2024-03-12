#pragma once

#include "entities/player.h"
#include "entities/platform.h"
#include "entities/trigger.h"

#include <string>
#include <vector>

namespace level {

// Levelizer is a way to serialize and deserialize .lvl files. 
// Will help with writing to and reading from .lvl files, retrieving 
// the correct type of data as it extracts them from the file.

// Levelizer functions
//////////////////////////////////////////////
void levelizer_create(const std::string& path);

void levelizer_save_player(const entities::player_t& player);
void levelizer_load_player(entities::player_t& out_player);

void levelizer_save_platforms(const std::vector<entities::platform_t>& platforms);
void levelizer_load_platforms(std::vector<entities::platform_t>& out_platforms);

void levelizer_save_triggers(const std::vector<entities::trigger_t>& triggers);
void levelizer_load_triggers(std::vector<entities::trigger_t>& out_triggers);
//////////////////////////////////////////////

}
