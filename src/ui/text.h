#pragma once

#include <raylib.h>

#include <string>

namespace ui {

// Text type
////////////////////////////////////////////////////
struct text_t {
  std::string str;
  int font_size;
  Vector2 position, size, offset; 
  Color color;
};
////////////////////////////////////////////////////

// Text functions
////////////////////////////////////////////////////
text_t text_create(const std::string& str, int font_size, Vector2 position,  Color col, int alpha);
void text_change(text_t& text, const std::string& new_str); 
void text_fade_in(text_t& text);
void text_fade_out(text_t& text);
void text_render(text_t& text);
////////////////////////////////////////////////////

}
