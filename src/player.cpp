#include "../inc/player.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <raylib.h>

void Player::input(ECS &ecs)
{
  Velocity b_v {0, 0};
  Velocity h_l_v = (*ecs.velocities.getComponent(this->hand_l));
  Velocity h_r_v = (*ecs.velocities.getComponent(this->hand_r));

  if (IsKeyDown(KEY_SPACE)) switch (this->active_hand)
  {
    case Hand::left:
      this->active_hand = Hand::right;
      break;
    case Hand::right:
      this->active_hand = Hand::left;
      break;
  };

  if (IsKeyDown(KEY_UP)) b_v.vy = -2.0f;
  if (IsKeyDown(KEY_LEFT)) b_v.vx = -2.0f;
  if (IsKeyDown(KEY_DOWN)) b_v.vy = 2.0f;
  if (IsKeyDown(KEY_RIGHT)) b_v.vx = 2.0f;

  switch (this->active_hand)
  {
    case Hand::left: 
      if (IsKeyDown(KEY_W)) h_l_v.vy -= 1.0f;
      if (IsKeyDown(KEY_A)) h_l_v.vx -= 1.0f;
      if (IsKeyDown(KEY_S)) h_l_v.vy += 1.0f;
      if (IsKeyDown(KEY_D)) h_l_v.vx += 1.0f;
      break;
    case Hand::right: 
      if (IsKeyDown(KEY_W)) h_r_v.vy -= 1.0f;
      if (IsKeyDown(KEY_A)) h_r_v.vx -= 1.0f;
      if (IsKeyDown(KEY_S)) h_r_v.vy += 1.0f;
      if (IsKeyDown(KEY_D)) h_r_v.vx += 1.0f;
      break;
  }

  ecs.velocities.set_component(this->body, b_v);
  ecs.velocities.set_component(this->hand_l, h_l_v);
  ecs.velocities.set_component(this->hand_r, h_r_v);
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
    // std::cout << "Left hand angle: " << atan2(h_l_pos.y - b_pos.y, h_l_pos.x - b_pos.x) << std::endl;

    // Temporary dont ask
    (*ecs.velocities.getComponent(this->hand_l)).vx = 0.0;
    (*ecs.velocities.getComponent(this->hand_l)).vy = 0.0;
  }

  if (h_r_dis > this->max_d) // Right hand too far away
  {
    // std::cout << "Right hand angle: " << atan2(h_r_pos.y - b_pos.y, h_r_pos.x - b_pos.x) << std::endl;

    // Temporary dont ask
    (*ecs.velocities.getComponent(this->hand_r)).vx = 0.0;
    (*ecs.velocities.getComponent(this->hand_r)).vy = 0.0;
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

  Velocity vel = { 0.0f, 0.0f };
  Dimension dim = { 32.0f, 32.0f };
  Collider collider;
  collider.update(b_pos, dim);

  ecs.positions.insert(body, b_pos);
  ecs.velocities.insert(body, vel);
  ecs.dimensions.insert(body, dim);
  ecs.colliders.insert(body, collider);
  ecs.sprites.insert(body, Sprite{body_tex});

  // Left hand
  {
    Position pos = { 
      static_cast<float>(b_pos.x + rand() % 10), 
      static_cast<float>(b_pos.y + rand() % 10) 
    };

    Velocity vel = { 0.0f, 0.0f };
    Dimension dim = { 32.0f, 32.0f };
    Collider collider;
    collider.update(pos, dim);

    ecs.positions.insert(hand_l, pos);
    ecs.velocities.insert(hand_l, vel);
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
    Dimension dim = { 32.0f, 32.0f };
    Collider collider;
    collider.update(pos, dim);

    ecs.positions.insert(hand_r, pos);
    ecs.velocities.insert(hand_r, vel);
    ecs.dimensions.insert(hand_r, dim);
    ecs.colliders.insert(hand_r, collider);
    ecs.sprites.insert(hand_r, Sprite{hand_r_tex});
  }

  return Player{ body, hand_l, hand_r, 2, 200, Hand::left };
}

