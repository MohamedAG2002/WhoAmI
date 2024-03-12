#include "dialogue_manager.h"
#include "collision/collision_space.h"
#include "entities/trigger.h"
#include "ui/text.h"

#include <raylib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace dialogue {

// Dialogue manager type 
/////////////////////////////////////////////////////
struct dialogue_manager_t {
  std::vector<std::string> dialogue;
  std::vector<ui::text_t> texts;

  std::string path;

  int total_text;
};

static dialogue_manager_t* s_man;
/////////////////////////////////////////////////////

// Private functions
/////////////////////////////////////////////////////
static const std::string read_dialogue_file() {
  std::stringstream str;
  std::fstream file(s_man->path, std::ios::in);
  if(!file.is_open()) {
    std::cerr << "[FILE-ERROR]: Could not open dialogue file at \'" << s_man->path << "\'\n";
    return "";
  } 
  
  str << file.rdbuf();
  file.close();

  return str.str();
}

static void fill_dialogue_vector() {
  std::string str = read_dialogue_file();
  std::string text;

  for(int i = 0; i < str.size(); i++) {
    if(str[i] != '*') {
      text += str[i];
    }
    else {
      s_man->dialogue.push_back(text);
      text = "";
      continue;
    }
  }
}
/////////////////////////////////////////////////////

// Callbacks
/////////////////////////////////////////////////////
void add_dialogue_callback(const entities::trigger_t& trigger) {
  if(trigger.type != entities::TRIGG_TYPE_TEXT) {
    return;
  }

  Vector2 pos = Vector2(trigger.position.x + trigger.size.x / 2.0f, trigger.position.y - 56.0f);
  s_man->texts.push_back(ui::text_create(s_man->dialogue[s_man->total_text++], 15, pos, YELLOW, 0));
}
/////////////////////////////////////////////////////

// Dialogue manager functions
/////////////////////////////////////////////////////
void create(const std::string& path) {
  s_man = new dialogue_manager_t();
 
  s_man->total_text = 0;
  s_man->path = path;
  fill_dialogue_vector();

  collision::add_trigger_callback(add_dialogue_callback);
}

void destroy() {
  s_man->texts.clear();
  s_man->dialogue.clear();

  delete s_man;
}

void render() {
  for(auto& text : s_man->texts) {
    ui::text_fade_in(text);
    ui::text_render(text); 
  }
}

void reset() {
  s_man->total_text = 0;

  s_man->texts.clear();
}
/////////////////////////////////////////////////////

}
