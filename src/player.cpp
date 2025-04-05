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

  ecs.accelerations.setComponent(this->body, b_a);
  ecs.accelerations.setComponent(this->hand_l, h_l_a);
  ecs.accelerations.setComponent(this->hand_r, h_r_a);
}

void Player::update(ECS& ecs)
{
  Position b_pos = (*ecs.positions.getComponent(this->body));
  Position h_l_pos = (*ecs.positions.getComponent(this->hand_l));
  Position h_r_pos = (*ecs.positions.getComponent(this->hand_r));

  float h_l_dis = (float) sqrt(pow(h_l_pos.x - b_pos.x, 2) + pow(h_l_pos.y - b_pos.y, 2)); 
  float h_r_dis = (float) sqrt(pow(h_r_pos.x - b_pos.x, 2) + pow(h_r_pos.y - b_pos.y, 2)); 

  if (h_l_dis > this->max_d) // Left hand too far away
  {

    Position new_pos {
      (b_pos.x + this->max_d * ((h_l_pos.x - b_pos.x ) / h_l_dis)),
      (b_pos.y + this->max_d * ((h_l_pos.y - b_pos.y ) / h_l_dis))
    };

    // printf("Right pos: (%f, %f)\n\t- %f\n", new_pos.x, new_pos.y, h_l_dis);

    // double angle = atan2(h_l_pos.y - b_pos.y, h_l_pos.x - b_pos.x);

    ecs.positions.setComponent(this->hand_l, new_pos);
  }

  if (h_r_dis > this->max_d) // Right hand too far away
  {

    Position new_pos {
      (b_pos.x + this->max_d * ((h_r_pos.x - b_pos.x ) / h_r_dis)),
      (b_pos.y + this->max_d * ((h_r_pos.y - b_pos.y ) / h_r_dis))
    };

    // printf("Right pos: (%f, %f)\n\t- %f\n", new_pos.x, new_pos.y, h_l_dis);

    // double angle = atan2(h_r_pos.y - b_pos.y, h_r_pos.x - b_pos.x);
    ecs.positions.setComponent(this->hand_r, new_pos);
  }
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

  return Player{
    body,
    hand_l,
    hand_r,
    4,
    2,
    200,
    true,
  };
}

