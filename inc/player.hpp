#include "ecs.hpp"
#include "entityType.hpp"

struct Player
{
	Entity id;

  void input(ECS& ecs);
};

Player init_player(ECS &ecs);

