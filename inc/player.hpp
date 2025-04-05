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
  float max_d;

	int gamepad_id;

  bool using_left;

  void input(ECS& ecs);
  void update(ECS& ecs);
	void drawArms(ECS& ecs);
  void gamepad_input(ECS&, Acceleration&, Acceleration&, Acceleration&);
};

Player init_player(ECS &);

