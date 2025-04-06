#include "../inc/player.hpp"
#include <cmath>
#include <cstdio>
#include <raylib.h>


#define DASH_COOLDOWN 60
#define GLOVE_BOUNCE_BACK 3


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

  if (buttonFour and this->dashCooldown == 0)
  {
		this->dashCooldown = DASH_COOLDOWN;
    b.accX *= 30;
    b.accY *= 30;
		playRandomWoosh();
  }


  if (leftTrigger)
  {
    if (this->left_dissabled) 
    {
      if (rightStickX == 0.0f && rightStickY == 0.0f) {
        this->left_dissabled = false;
      }
    }
    else 
    {
      l.accX = rightStickX * this->hand_acc;
      l.accY = rightStickY * this->hand_acc;
    }
  }
  
  if (rightTrigger)
  {
    if (this->right_dissabled) 
    {
      if (rightStickX == 0.0f && rightStickY == 0.0f) {
        this->right_dissabled = false;
      }
    }
    else 
    {
      r.accY = rightStickY * this->hand_acc;
      r.accX = rightStickX * this->hand_acc;
    }
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
  
  ecs.accelerations.setComponent(this->body, b_a);
  ecs.accelerations.setComponent(this->left, h_l_a);
  ecs.accelerations.setComponent(this->right, h_r_a);
}

void Player::update(ECS& ecs)
{
  // Body
  Dimension b_dim = (*ecs.dimensions.getComponent(this->body));
  Position b_pos = (*ecs.positions.getComponent(this->body));
  Position b_center {
    b_pos.x + b_dim.w / 2.0f,
    b_pos.y + b_dim.h / 2.0f
  };

  // Left hand
  Position h_l_pos = (*ecs.positions.getComponent(this->left));
  Dimension h_l_dim = (*ecs.dimensions.getComponent(this->left));
  Velocity h_l_v(*ecs.velocities.getComponent(this->left));
  Position h_l_center {
    h_l_pos.x + h_l_dim.w / 2.0f,
    h_l_pos.y + h_l_dim.h / 2.0f
  };
  float h_l_dis = (float) sqrt(pow(h_l_center.x - b_pos.x, 2) + pow(h_l_center.y - b_center.y, 2)); 

  // Right hand
  Position h_r_pos = (*ecs.positions.getComponent(this->right));
  Dimension h_r_dim = (*ecs.dimensions.getComponent(this->right));
  Velocity h_r_v(*ecs.velocities.getComponent(this->right));
  Position h_r_center {
    h_r_pos.x + h_r_dim.w / 2.0f,
    h_r_pos.y + h_r_dim.h / 2.0f
  };
  float h_r_dis = (float) sqrt(pow(h_r_center.x - b_pos.x, 2) + pow(h_r_center.y - b_center.y, 2)); 

  if (h_l_dis > this->max_d) // Left hand too far away
  {
    Acceleration acc = (*ecs.accelerations.getComponent(this->left));

    Vector2 v {
      (h_l_center.x - b_center.x ) / h_l_dis,
      (h_l_center.y - b_center.y ) / h_l_dis
    };

    if (acc.accX != 0.0f || acc.accY != 0.0f)
    {
      float acc_t = sqrt(pow(acc.accX, 2) + pow(acc.accY, 2));
      acc.accX = acc_t * GLOVE_BOUNCE_BACK * (-v.x);
      acc.accY = acc_t * GLOVE_BOUNCE_BACK * (-v.y);

      this->left_dissabled = true;
    }

    Position new_pos {
      (b_center.x + this->max_d * v.x - h_l_dim.w / 2),
      (b_center.y + this->max_d * v.y - h_l_dim.h / 2)
    };

    ecs.positions.setComponent(this->left, new_pos);
    ecs.accelerations.setComponent(this->left, acc);
  }
  
  if (h_r_dis > this->max_d) // Right hand too far away
  {
    Acceleration acc = (*ecs.accelerations.getComponent(this->right));

    Vector2 v {
      (h_r_center.x - b_center.x ) / h_r_dis,
      (h_r_center.y - b_center.y ) / h_r_dis
    };

    if (!(acc.accX == 0.0f && acc.accY == 0.0f))
    {
      float acc_t = sqrt(pow(acc.accX, 2) + pow(acc.accY, 2));
      acc.accX = acc_t * GLOVE_BOUNCE_BACK * (-v.x);
      acc.accY = acc_t * GLOVE_BOUNCE_BACK * (-v.y);

      this->right_dissabled = true;
    }

    Position new_pos {
      (b_center.x + this->max_d * v.x - h_r_dim.w / 2),
      (b_center.y + this->max_d * v.y - h_r_dim.h / 2)
    };

    ecs.positions.setComponent(this->right, new_pos);
    ecs.accelerations.setComponent(this->right, acc);
  }

	if (this->dashCooldown > 0) this->dashCooldown--;

  Acceleration b_acc = (*ecs.accelerations.getComponent(this->body));
  Acceleration l_acc {
    (*ecs.accelerations.getComponent(this->left)).accX + b_acc.accX,
    (*ecs.accelerations.getComponent(this->left)).accY + b_acc.accY
  };
  Acceleration r_acc {
    (*ecs.accelerations.getComponent(this->right)).accX + b_acc.accX,
    (*ecs.accelerations.getComponent(this->right)).accY + b_acc.accY
  };

  ecs.accelerations.setComponent(this->left, l_acc);
  ecs.accelerations.setComponent(this->right, r_acc);

}


Player init_player(ECS& ecs, Position& p_pos, Texture2D& gloveTex, Texture2D& ratTex) 
{
	Entity bodyId = ecs.createEntity();		
	Entity leftId = ecs.createEntity();		
	Entity rightId = ecs.createEntity();

	// Body
	{
		Position pos = p_pos;
  	Velocity vel = { 0.0f, 0.0f, 2.0f, 0.0f };
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
  	Velocity vel = { 0.0f, 0.0f, 7.0f, 0.0f };
  	Acceleration acc = { 0.0f, 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 32.0f };
		Mass mass = { 600.0f };

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
  	Velocity vel = { 0.0f, 0.0f, 7.0f, 0.0f };
  	Acceleration acc = { 0.0f, 0.0f, 0.0f };
  	Dimension dim = { 32.0f, 32.0f };
		Mass mass = { 600.0f };

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
    2,          		  // body acc
    15,         		  // hand acc
		0,							  // dash cooldown
    0.5,        		  // hand retard
    0.95,       		  // body retard
    30,          		  // hand min dist
    100,          		  // hand max dist
    0,          		  // score
    p_pos,            // spawn position
    next_gamepad_id++,
    true,             // starting hand (true: left, false: right)
    false,            // Disable left hand
    false,            // Disable right hand
  };
}

void Player::respawn(ECS& ecs) {
  Position pos = this->spawn_pos;
  Velocity vel {0, 0};
  ecs.positions.setComponent(this->body, pos);  
  ecs.velocities.setComponent(this->body, vel);  

  pos.x += 40.0f;
  ecs.positions.setComponent(this->right, pos);  
  ecs.velocities.setComponent(this->right, vel);  

	this->dashCooldown = 0;
  pos.x -= 80.0f;
  ecs.positions.setComponent(this->left, pos);  
  ecs.velocities.setComponent(this->left, vel);  
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

  score_text += ": $ ";
  score_text += std::to_string(10*this->score);

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
