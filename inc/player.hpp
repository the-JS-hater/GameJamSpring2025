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

  size_t max_v;
  size_t max_d;

  enum Hand active_hand;

  void input(ECS& ecs);
  void update(ECS& ecs);
};

Player init_player(ECS &ecs);

