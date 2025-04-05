#include "player.hpp"
#include <raylib.h>
#include <raylib.h>
#include <string>



struct VictoryScreen {
  Rectangle bounds;

  VictoryScreen(float width, float height);

  void print_screen(Player&, Player&);
  void input(Player&, Player&, bool&);
};
