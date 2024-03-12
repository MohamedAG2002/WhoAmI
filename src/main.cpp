#include "core/game.h"

int main() {
  game::create("Who Am I?", 1280, 720);
  game::run();
  game::destroy();
}
