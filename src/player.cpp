#include "../inc/player.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <raylib.h>

void Player::input(ECS &ecs)
{
  Acceleration b_a {0, 0};
  Acceleration h_l_a = {0, 0};
  Acceleration h_r_a = {0, 0};

  if (IsGamepadAvailable(this->gamepad_id)) {
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;

    float leftStickX = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_LEFT_Y);
    float rightStickX = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_RIGHT_X);
    float rightStickY = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_RIGHT_Y);
    float leftTrigger = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_LEFT_TRIGGER);
    float rightTrigger = GetGamepadAxisMovement(gamepad_id, GAMEPAD_AXIS_RIGHT_TRIGGER);
    
    bool rightDownButton = IsGamepadButtonDown(gamepad_id, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    
    if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
    if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
    if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
    if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;

    b_a.accX=leftStickX * this->body_acc;
    b_a.accY=leftStickY * this->body_acc;
   
    if (rightDownButton) printf("Hell yeah\n");

    if (this->using_left) {
      h_l_a.accX=rightStickX * this->hand_acc;
      h_l_a.accY=rightStickY * this->hand_acc;
    }
    else
    {
      h_r_a.accX=rightStickX * this->hand_acc;
      h_r_a.accY=rightStickY * this->hand_acc;
    }
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
  ecs.accelerations.setComponent(this->hand_l, h_l_a);
  ecs.accelerations.setComponent(this->hand_r, h_r_a);
}

void Player::update(ECS& ecs)
{
  Position b_pos = (*ecs.positions.getComponent(this->body));
  Position h_l_pos = (*ecs.positions.getComponent(this->hand_l));
  Position h_r_pos = (*ecs.positions.getComponent(this->hand_r));

  Velocity h_l_v = (*ecs.velocities.getComponent(this->hand_l));

  float h_l_dis = (float) sqrt(pow(h_l_pos.x - b_pos.x, 2) + pow(h_l_pos.y - b_pos.y, 2)); 
  float h_r_dis = (float) sqrt(pow(h_r_pos.x - b_pos.x, 2) + pow(h_r_pos.y - b_pos.y, 2)); 

  float angle_l = atan2(h_l_pos.y - b_pos.y, h_l_pos.x - b_pos.x);
  float angle_r = atan2(h_r_pos.y - b_pos.y, h_r_pos.x - b_pos.x);

  if (h_l_dis > this->max_d) {



  }

  // printf("Left angle: %f\n", angle_l);
  // printf("Right angle: %f\n", angle_r);
}

Player init_player(ECS& ecs) {
	Texture2D body_tex = LoadTexture("resources/sprites/Spam.png");
	Texture2D hand_l_tex = LoadTexture("resources/sprites/Spam.png");
	Texture2D hand_r_tex = LoadTexture("resources/sprites/Spam.png");
	Entity body = ecs.createEntity();
	Entity hand_l = ecs.createEntity();
	Entity hand_r = ecs.createEntity();

  // Body
  Position b_pos = { 
    static_cast<float>(rand() % GetScreenWidth()), 
    static_cast<float>(rand() % GetScreenHeight()) 
  };

	// Body
	{
  	Velocity vel = { 0.0f, 0.0f };
  	Acceleration acc = { 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 32.0f };
  	Collider collider;
  	collider.update(b_pos, dim);

  	ecs.positions.insert(body, b_pos);
  	ecs.velocities.insert(body, vel);
  	ecs.accelerations.insert(body, acc);
  	ecs.dimensions.insert(body, dim);
  	ecs.colliders.insert(body, collider);
  	ecs.sprites.insert(body, Sprite{body_tex});
	}

  // Left hand
  {
    Position pos = { 
      static_cast<float>(b_pos.x + rand() % 10), 
      static_cast<float>(b_pos.y + rand() % 10) 
    };

    Velocity vel = { 0.0f, 0.0f };
    Acceleration acc = { 0.0f, 0.0f };
    Dimension dim = { 32.0f, 32.0f };
    Collider collider;
    collider.update(pos, dim);

    ecs.positions.insert(hand_l, pos);
    ecs.velocities.insert(hand_l, vel);
    ecs.accelerations.insert(hand_l, acc);
    ecs.dimensions.insert(hand_l, dim);
    ecs.colliders.insert(hand_l, collider);
    ecs.sprites.insert(hand_l, Sprite{hand_l_tex});
  }

  // Right hand
  {
    Position pos = { 
      static_cast<float>(b_pos.x + rand() % 10), 
      static_cast<float>(b_pos.y + rand() % 10) 
    };

    Velocity vel = { 0.0f, 0.0f };
    Acceleration acc = { 0.0f, 0.0f };
    Dimension dim = { 32.0f, 32.0f };
    Collider collider;
    collider.update(pos, dim);

    ecs.positions.insert(hand_r, pos);
    ecs.velocities.insert(hand_r, vel);
    ecs.accelerations.insert(hand_r, acc);
    ecs.dimensions.insert(hand_r, dim);
    ecs.colliders.insert(hand_r, collider);
    ecs.sprites.insert(hand_r, Sprite{hand_r_tex});
  }

	static int next_gamepad_id{};
  return Player{
    body,
    hand_l,
    hand_r,
    4,
    2,
    200,
    next_gamepad_id++,
    true,
  };
}

