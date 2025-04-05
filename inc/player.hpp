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

  double acc;
  size_t max_v;
  size_t max_d;

  bool is_moving;
  bool using_left;

  void input(ECS& ecs);
  void update(ECS& ecs);
};

Player init_player(ECS &ecs);

