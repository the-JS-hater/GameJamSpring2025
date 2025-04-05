#include "../inc/player.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <string>

void Player::gamepad_input(Acceleration& b, Acceleration& l, Acceleration& r) {
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
  bool buttonFour = IsGamepadButtonDown(gamepad_id, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);

  if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
  if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
  if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
  if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;

  b.accX = leftStickX * this->body_acc;
  b.accY = leftStickY * this->body_acc;

  if (buttonFour)
  {
    b.accX *= 10;
    b.accY *= 10;
  }

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
    gamepad_input(b_a, h_l_a, h_r_a);
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

  float h_l_dis = (float) sqrt(pow(h_l_pos.x - b_pos.x, 2) + pow(h_l_pos.y - b_pos.y, 2)); 
  float h_r_dis = (float) sqrt(pow(h_r_pos.x - b_pos.x, 2) + pow(h_r_pos.y - b_pos.y, 2)); 

  if (h_l_dis > this->max_d) // Left hand too far away
  {

    Position new_pos {
      (b_pos.x + this->max_d * (h_l_pos.x - b_pos.x ) / h_l_dis),
      (b_pos.y + this->max_d * (h_l_pos.y - b_pos.y ) / h_l_dis)
    };

    ecs.positions.setComponent(this->left, new_pos);
  }

  if (h_r_dis > this->max_d) // Right hand too far away
  {

    Position new_pos {
      (b_pos.x + this->max_d * (h_r_pos.x - b_pos.x ) / h_r_dis),
      (b_pos.y + this->max_d * (h_r_pos.y - b_pos.y ) / h_r_dis)
    };

    ecs.positions.setComponent(this->right, new_pos);
  }
}


Player init_player(ECS& ecs, Position& p_pos, Texture2D& gloveTex, Texture2D& ratTex) 
{
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
    2,          // body acc
    15,         // hand acc
    0.5,        // hand retard
    0.95,       // body retard
    200,        // hand max dist
    0,          // score
    p_pos,
    next_gamepad_id++,
    true,
  };
}

void Player::respawn(ECS& ecs) {
  Position pos = this->spawn_pos;
  Velocity vel {0, 0};
  ecs.positions.setComponent(this->body, pos);  
  ecs.velocities.setComponent(this->body, vel);  

  pos.x += 32.0f;
  ecs.positions.setComponent(this->left, pos);  
  ecs.velocities.setComponent(this->left, vel);  

  pos.x -= 64.0f;
  ecs.positions.setComponent(this->right, pos);  
  ecs.velocities.setComponent(this->right, vel);  
}

void Player::draw_score(int player, int height) {
  std::string score_text = "Player ";

  if (player == 1) 
  {
    score_text += "ONE";  
  }
  else 
  {
    score_text += "TWO";  
  }

  score_text += ": ";
  score_text += std::to_string(this->score);

  DrawText(score_text.c_str(), 20, height, 32, BLACK);
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
	Vector2 lbPoint = { bPos->x + 5.f, bPos->y + bDim->h / 2.0f };
	Vector2 rbPoint = { bPos->x + bDim->w - 5.f, bPos->y + bDim->h / 2.0f };

	DrawLineV(lPoint, lbPoint, BROWN);  
	DrawLineV(rPoint, rbPoint, BROWN); 
}
