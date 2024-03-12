#pragma once

#include <string>

namespace dialogue {

// Dialogue manager functions
/////////////////////////////////////////////////////
void create(const std::string& path);
void destroy();
void render();
void reset();
/////////////////////////////////////////////////////

}
