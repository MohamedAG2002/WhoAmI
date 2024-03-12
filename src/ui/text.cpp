#include "text.h"

#include <raylib.h>

#include <cmath>
#include <string>

namespace ui {

// Text functions
////////////////////////////////////////////////////
text_t text_create(const std::string& str, int font_size, Vector2 position, Color col, int alpha) {
  return text_t {
    .str = str, 
    .font_size = font_size, 
    .position = position,
    .size = MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f), 
    .offset = Vector2(0.0f, 0.0f), 
    .color = Color(col.r, col.g, col.b, alpha),
  };
}

void text_change(text_t& text, const std::string& new_str) {
  text.str = new_str;
  text.size = MeasureTextEx(GetFontDefault(), new_str.c_str(), text.font_size, 1.0f); 
}

void text_fade_in(text_t& text) {
  // There's nothing to do if the text is fully transparent
  if(text.color.a >= 255) {
    return; 
  }

  text.color.a = (unsigned char)std::lerp(text.color.a, 255, GetFrameTime()); 
}

void text_fade_out(text_t& text) {
  // There's nothing to do if the text's alpha is non-existent
  if(text.color.a == 0) {
    return; 
  }

  text.color.a = (unsigned char)std::lerp(text.color.a, 0, GetFrameTime()); 
}

void text_render(text_t& text) {
  DrawTextEx(GetFontDefault(), 
             text.str.c_str(), 
             Vector2(text.position.x - text.size.x / 2.0f, text.position.y - text.size.y / 2.0f), 
             text.font_size, 
             1.0f, 
             text.color); 
}
////////////////////////////////////////////////////

}
