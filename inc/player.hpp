#include "components.hpp"
#include "ecs.hpp"
#include "entityType.hpp"

enum Hand {
  left,
  right
};

struct Player
{
	Entity body;
	Entity left;
	Entity right;

  float body_acc;
  float hand_acc;

  float hand_retarding_factor;
  float body_retarding_factor;
  float max_d;

  int score;

	int gamepad_id;
  bool using_left;

  void input(ECS& ecs);
  void update(ECS& ecs);
	void drawArms(ECS& ecs);
  void gamepad_input(Acceleration&, Acceleration&, Acceleration&);
  void draw_score(int, int);
};

Player init_player(ECS &, Position&, Texture2D&, Texture2D&);

