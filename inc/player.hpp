#include "ecs.hpp"
#include "entityType.hpp"
#include <cstddef>

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
  float max_d;

	int gamepad_id;

  bool using_left;

  void input(ECS& ecs);
  void update(ECS& ecs);
	void drawArms(ECS& ecs);
};

Player init_player(ECS &ecs);

