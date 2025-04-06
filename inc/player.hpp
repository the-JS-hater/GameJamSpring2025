#pragma once

#include "components.hpp"
#include "ecs.hpp"
#include "entityType.hpp"
#include "audioManager.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <string>


struct Player
{
	Entity body;
	Entity left;
	Entity right;

  float body_acc;
  float hand_acc;

	int dashCooldown;

  float hand_retarding_factor;
  float body_retarding_factor;
  float min_d;
  float max_d;

  int score;
  Position spawn_pos;

	int gamepad_id;
  bool using_left;
  bool left_dissabled;
  bool right_dissabled;

  void input(ECS& ecs);
  void update(ECS& ecs);
  void respawn(ECS& ecs);
	void drawArms(ECS& ecs);
  void gamepad_input(Acceleration&, Acceleration&, Acceleration&);
  void draw_score(int, int);
};

Player init_player(ECS &, Position&, Texture2D&, Texture2D&);

