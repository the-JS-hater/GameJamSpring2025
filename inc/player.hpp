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
	Entity hand_l;
	Entity hand_r;

  float body_acc;
  float hand_acc;
  float max_d;

	int gamepad_id;

  bool using_left;

  void input(ECS& ecs);
  void update(ECS& ecs);
};

Player init_player(ECS &ecs);

