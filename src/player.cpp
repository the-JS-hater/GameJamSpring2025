#include "../inc/player.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <raylib.h>

void Player::gamepad_input(ECS& ecs, Acceleration& b, Acceleration& l, Acceleration& r) {
  const float leftStickDeadzoneX = 0.1f;
  const float leftStickDeadzoneY = 0.1f;
  const float rightStickDeadzoneX = 0.1f;
  const float rightStickDeadzoneY = 0.1f;

  float leftStickX = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_LEFT_X);
  float leftStickY = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_LEFT_Y);
  float rightStickX = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_RIGHT_X);
  float rightStickY = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_RIGHT_Y);

  bool leftTrigger = IsGamepadButtonDown(gamepad_id, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
  bool rightTrigger = IsGamepadButtonDown(gamepad_id, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);

  if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
  if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
  if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
  if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;

  b.accX = leftStickX * this->body_acc;
  b.accY = leftStickY * this->body_acc;

  if (leftTrigger)
  {
    l.accX = rightStickX * this->hand_acc;
    l.accY = rightStickY * this->hand_acc;
  }
  
  if (rightTrigger)
  {
    r.accX = rightStickX * this->hand_acc;
    r.accY = rightStickY * this->hand_acc;
  }
}

void Player::input(ECS &ecs)
{
  Acceleration b_a {0, 0, this->body_retarding_factor};
  Acceleration h_l_a = {0, 0, this->hand_retarding_factor};
  Acceleration h_r_a = {0, 0, this->hand_retarding_factor};

  if (IsGamepadAvailable(this->gamepad_id)) 
  {
    gamepad_input(ecs, b_a, h_l_a, h_r_a);
  }
  else
  {
    if (IsKeyDown(KEY_SPACE)) this->using_left = !this->using_left;

    if (IsKeyDown(KEY_UP)) b_a.accY = -this->body_acc;
    if (IsKeyDown(KEY_LEFT)) b_a.accX = -this->body_acc;
    if (IsKeyDown(KEY_DOWN)) b_a.accY = this->body_acc;
    if (IsKeyDown(KEY_RIGHT)) b_a.accX = this->body_acc;

    if (this->using_left)
    {
      if (IsKeyDown(KEY_W)) h_l_a.accY = -this->hand_acc;
      if (IsKeyDown(KEY_A)) h_l_a.accX = -this->hand_acc;
      if (IsKeyDown(KEY_S)) h_l_a.accY = this->hand_acc;
      if (IsKeyDown(KEY_D)) h_l_a.accX = this->hand_acc;
    }
    else 
    {
      if (IsKeyDown(KEY_W)) h_r_a.accY = -this->hand_acc;
      if (IsKeyDown(KEY_A)) h_r_a.accX = -this->hand_acc;
      if (IsKeyDown(KEY_S)) h_r_a.accY = this->hand_acc;
      if (IsKeyDown(KEY_D)) h_r_a.accX = this->hand_acc;
    }
  }
  ecs.accelerations.setComponent(this->body, b_a);
  ecs.accelerations.setComponent(this->left, h_l_a);
  ecs.accelerations.setComponent(this->right, h_r_a);
}

void Player::update(ECS& ecs)
{
  Position b_pos = (*ecs.positions.getComponent(this->body));
  Position h_l_pos = (*ecs.positions.getComponent(this->left));
  Position h_r_pos = (*ecs.positions.getComponent(this->right));

  Velocity h_l_v = (*ecs.velocities.getComponent(this->left));

  float h_l_dis = (float) sqrt(pow(h_l_pos.x - b_pos.x, 2) + pow(h_l_pos.y - b_pos.y, 2)); 
  float h_r_dis = (float) sqrt(pow(h_r_pos.x - b_pos.x, 2) + pow(h_r_pos.y - b_pos.y, 2)); 

  float angle_l = atan2(h_l_pos.y - b_pos.y, h_l_pos.x - b_pos.x);
  float angle_r = atan2(h_r_pos.y - b_pos.y, h_r_pos.x - b_pos.x);

  if (h_l_dis > this->max_d) {



  }

  // printf("Left angle: %f\n", angle_l);
  // printf("Right angle: %f\n", angle_r);
}


Player init_player(ECS& ecs, Position& p_pos) 
{
	Texture2D gloveTex = LoadTexture("resources/sprites/Glove.png");
	Texture2D ratTex = LoadTexture("resources/sprites/Rat.png");
	
	Entity bodyId = ecs.createEntity();		
	Entity leftId = ecs.createEntity();		
	Entity rightId = ecs.createEntity();

	// Body
	{
		Position pos = p_pos;
  	Velocity vel = { 0.0f, 0.0f, 2.0f };
  	Acceleration acc = { 0.0f, 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 64.0f };
		Mass mass = { 250.0f };

  	Collider collider;
  	collider.update(pos, dim);

  	ecs.positions.insert(bodyId, pos);
  	ecs.velocities.insert(bodyId, vel);
  	ecs.accelerations.insert(bodyId, acc);
  	ecs.dimensions.insert(bodyId, dim);
  	ecs.colliders.insert(bodyId, collider);
  	ecs.sprites.insert(bodyId, Sprite{ ratTex });
		ecs.masses.insert(bodyId, mass);
	}
	// Glove left 
	{
		Position pos = {
			p_pos.x - 32.0f,
      p_pos.y
		};
  	Velocity vel = { 0.0f, 0.0f, 7.0f };
  	Acceleration acc = { 0.0f, 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 32.0f };
		Mass mass = { 10000.0f };

  	Collider collider;
  	collider.update(pos, dim);

  	ecs.positions.insert(leftId, pos);
  	ecs.velocities.insert(leftId, vel);
  	ecs.accelerations.insert(leftId, acc);
  	ecs.dimensions.insert(leftId, dim);
  	ecs.colliders.insert(leftId, collider);
  	ecs.sprites.insert(leftId, Sprite{ gloveTex });
		ecs.masses.insert(leftId, mass);
	}
	// Glove right 
	{
		Position pos = {
			p_pos.x + 32.0f,
		  p_pos.y	
		};
  	Velocity vel = { 0.0f, 0.0f, 7.0f };
  	Acceleration acc = { 0.0f, 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 32.0f };
		Mass mass = { 10000.0f };

  	Collider collider;
  	collider.update(pos, dim);

  	ecs.positions.insert(rightId, pos);
  	ecs.velocities.insert(rightId, vel);
  	ecs.accelerations.insert(rightId, acc);
  	ecs.dimensions.insert(rightId, dim);
  	ecs.colliders.insert(rightId, collider);
  	ecs.sprites.insert(rightId, Sprite{ gloveTex });
		ecs.masses.insert(rightId, mass);
	}

	static int next_gamepad_id{};
  return Player{
    bodyId,
    leftId,
    rightId,
    2,
    10,
    0.70,
    0.95,
    200,
    next_gamepad_id++,
    true,
  };
}

void Player::drawArms(ECS& ecs)
{
	Position* lPos = ecs.positions.getComponent(left);
	Position* rPos = ecs.positions.getComponent(right);
	Position* bPos = ecs.positions.getComponent(body);
	
	if (!lPos || !rPos || !bPos) return;

	Dimension* lDim = ecs.dimensions.getComponent(left);
	Dimension* rDim = ecs.dimensions.getComponent(right);
	Dimension* bDim = ecs.dimensions.getComponent(body);
	
	if (!lDim || !rDim || !bDim) return;

	Vector2 lPoint = { lPos->x + lDim->w / 2.0f, lPos->y + lDim->h / 2.0f };
	Vector2 rPoint = { rPos->x + rDim->w / 2.0f, rPos->y + rDim->h / 2.0f };
	Vector2 bPoint = { bPos->x + bDim->w / 2.0f, bPos->y + bDim->h / 2.0f };

	DrawLineV(lPoint, bPoint, BROWN);  
	DrawLineV(rPoint, bPoint, BROWN); 
}
