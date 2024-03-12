#include "levelizer.h"
#include "collision/collider.h"
#include "entities/player.h"
#include "entities/platform.h"
#include "entities/trigger.h"

#include <raylib.h>

#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace level {

// Levelizer type
//////////////////////////////////////////////
struct levelizer_t {
  std::string path;
};

static levelizer_t s_lvl;
//////////////////////////////////////////////

// Private functions
//////////////////////////////////////////////
static void file_write_contents(const std::string& contents) {
  std::fstream file(s_lvl.path, std::ios::out | std::ios::app);
  if(!file.is_open()) {
    std::cerr << "[FILE-ERROR]: Could not open lvl file at \'" << s_lvl.path << "\'\n";
    return;
  }

  file.write((char*)contents.c_str(), contents.size());
  file.close();
}

static const std::string file_read_contents() {
  std::stringstream str; 
  std::fstream file(s_lvl.path, std::ios::in | std::ios::app);
  if(!file.is_open()) {
    std::cerr << "[FILE-ERROR]: Could not open lvl file at \'" << s_lvl.path << "\'\n";
    return "";
  }

  str << file.rdbuf();
  file.close();

  return str.str();
}

static void file_delete_contents() {
  std::fstream file(s_lvl.path, std::ios::out | std::ios::trunc);
  file.close();
}

static std::string get_iden_value(const std::string& unit, const std::string& iden) {
  return unit.substr(unit.find(iden) + iden.size(), unit.find(';') - iden.size());
}

static Vector2 get_unit_position(const std::string& unit) {
  Vector2 pos; 
  
  pos.x = std::stof(get_iden_value(unit, "  x = "));
  pos.y = std::stof(get_iden_value(unit, "  y = "));

  return pos;
}

static Color get_unit_color(const std::string& unit) {
  Color col; 
  
  col.r = std::stof(get_iden_value(unit, "  r = "));
  col.g = std::stof(get_iden_value(unit, "  g = "));
  col.b = std::stof(get_iden_value(unit, "  b = "));
  col.a = std::stof(get_iden_value(unit, "  a = "));

  return col;
}

static Vector2 get_unit_size(const std::string& unit) {
  Vector2 size; 
  
  size.x = std::stof(get_iden_value(unit, "  width = "));
  size.y = std::stof(get_iden_value(unit, "  height = "));

  return size;
}

static void set_unit_position(std::string& unit, Vector2 pos) {
  unit += "  x = " + std::to_string(pos.x) + ";\n";
  unit += "  y = " + std::to_string(pos.y) + ";\n";
}

static void set_unit_color(std::string& unit, Color color) {
  unit += "  r = " + std::to_string(color.r) + ";\n";
  unit += "  g = " + std::to_string(color.g) + ";\n";
  unit += "  b = " + std::to_string(color.b) + ";\n";
  unit += "  a = " + std::to_string(color.a) + ";\n";
}

static void set_unit_size(std::string& unit, Vector2 size) {
  unit += "  width = " + std::to_string(size.x) + ";\n";
  unit += "  height = " + std::to_string(size.y) + ";\n";
}
//////////////////////////////////////////////

// Levelizer functions
//////////////////////////////////////////////
void levelizer_create(const std::string& path) {
  s_lvl.path = path;
}

void levelizer_save_player(const entities::player_t& player) {
  std::string str;
  Vector2 pos = player.position;
  Color color = player.color;

  str += "player {\n"; 

  set_unit_position(str, pos);
  set_unit_color(str, color);

  str += "}\n";

  file_delete_contents(); // Start with a clean slate 
  file_write_contents(str);
}

void levelizer_load_player(entities::player_t& out_player) {
  std::string str = file_read_contents();
  std::string values;

  // Make sure the string isn't empty
  if(str.empty()) {
    std::cerr << "LEVELIZER-ERROR: Could not load player as the file \'" << s_lvl.path << "\' is empty\n";
    return;
  }

  // Get the values withing the '{}'
  int begin_player = strlen("player {\n");
  int end_player = str.find_first_of('}');
  values = str.substr(begin_player, end_player - 10);

  // Filling out the player with the given values
  out_player = entities::player_create(get_unit_position(values)); 
  out_player.color = get_unit_color(values);

  // Don't forget to update the collider
  collision::collider_update_vertices(out_player.collider);
}

void levelizer_save_platforms(const std::vector<entities::platform_t>& platforms) {
  std::string str; 
  str += "\nplatform_count = " + std::to_string(platforms.size()) + ";\n"; 

  for(int i = 0; i < platforms.size(); i++) {
    Vector2 pos = platforms[i].position;
    Vector2 size = platforms[i].size;
    Color color = platforms[i].color;

    str += "platform_" + std::to_string(i) + " {\n";
   
    set_unit_position(str, pos);
    set_unit_size(str, size);
    set_unit_color(str, color);

    str += "}\n"; 
  }

  file_write_contents(str);
}

void levelizer_load_platforms(std::vector<entities::platform_t>& out_platforms) {
  std::string str = file_read_contents(); 

  int plat_count = std::stoi(get_iden_value(str, "platform_count = "));
  if(plat_count == 0) {
    std::cerr << "LEVELIZER-ERROR: Could not load platforms as the file \'" << s_lvl.path << "\' is empty\n";
    return;
  }

  //out_platforms.resize(plat_count);
  for(int i = 0; i < plat_count; i++) {
    std::string plat_iden = "platform_" + std::to_string(i) + " {\n"; 

    int begin_plat = str.find(plat_iden) + plat_iden.size();
    int end_plat = str.find("}\n", begin_plat);
    std::string unit = str.substr(begin_plat, end_plat); 

    Vector2 pos = get_unit_position(unit);
    Vector2 size = get_unit_size(unit);
    Color color = get_unit_color(unit);
    
    out_platforms.push_back(entities::platform_create(pos, size, color));
  }
}

void levelizer_save_triggers(const std::vector<entities::trigger_t>& triggers) {
  std::string str;
  str += "\ntrigger_count = " + std::to_string(triggers.size()) + ";\n"; 

  for(int i = 0; i < triggers.size(); i++) {
    Vector2 pos = triggers[i].position;
    Vector2 size = triggers[i].size;
    int type = triggers[i].type; 
    bool active = triggers[i].is_active;

    str += "trigger_" + std::to_string(i) + " {\n";
   
    set_unit_position(str, pos);
    set_unit_size(str, size);

    str += "  type = " + std::to_string(type) + ";\n";
    str += "  active = " + std::to_string(active) + ";\n";

    str += "}\n"; 
  }

  file_write_contents(str);
}

void levelizer_load_triggers(std::vector<entities::trigger_t>& out_triggers) {
  std::string str = file_read_contents(); 

  int trig_count = std::stoi(get_iden_value(str, "trigger_count = "));
  if(trig_count == 0) {
    std::cerr << "LEVELIZER-ERROR: Could not load triggers as the file \'" << s_lvl.path << "\' is empty\n";
    return;
  }
  
  //out_triggers.resize(trig_count);
  for(int i = 0; i < trig_count; i++) {
    std::string trig_iden = "trigger_" + std::to_string(i) + " {\n"; 

    int begin_trig = str.find(trig_iden) + trig_iden.size();
    int end_trig = str.find("}\n", begin_trig);
    std::string unit = str.substr(begin_trig, end_trig); 

    Vector2 pos = get_unit_position(unit);
    Vector2 size = get_unit_size(unit);
    int trig_type = std::stoi(get_iden_value(unit, "  type = "));

    out_triggers.push_back(entities::trigger_create(pos, size, (entities::trigger_type_e)trig_type, true));
  }
}
//////////////////////////////////////////////

}
