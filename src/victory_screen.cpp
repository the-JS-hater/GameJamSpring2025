#include "../inc/victory_screen.hpp"
#include <raylib.h>
#include <string>

VictoryScreen::VictoryScreen(float width, float height) {
  this->bounds = Rectangle {0, 0, width, height};
}

void VictoryScreen::print_screen(Player& p_1, Player& p_2) {
  DrawRectangleRec(this->bounds, BLACK);
  DrawText("OUCH!", 30, 30, 64, WHITE);

  std::string text = "$ ";
  text += std::to_string(10 * p_1.score);
  text += " : $ ";
  text += std::to_string(10 * p_2.score);

  DrawText(text.c_str(), 30, 130, 64, WHITE);
  DrawText("Press X / A / 4? to Continue!", 30, 230, 64, WHITE);
}

void VictoryScreen::input(Player& p_1, Player& p_2, bool& is_win) {
  bool buttonFour = IsGamepadButtonDown(p_1.gamepad_id, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
  buttonFour = buttonFour || IsGamepadButtonDown(p_2.gamepad_id, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);

  if (buttonFour) {
    is_win = false;
  }
}
