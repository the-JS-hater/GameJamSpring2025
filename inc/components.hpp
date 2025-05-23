#pragma once

#include <raylib.h>
#include <functional>
#include "entityType.hpp"
	
using std::function;

struct Position 
{
    float x, y;
};

struct Velocity 
{
  float vx, vy, max_v, swap_max_v;
};

struct Dimension
{
	float w, h;
};

struct Mass
{
	float v;
};

struct Acceleration
{
	float accX, accY, retarding_factor;
};

struct Lifetime
{
	int timer;
};

struct Collider {
  Rectangle rect; 

  void update(Position const& pos, Dimension const& dim) 
	{
		rect.x = pos.x;
		rect.y = pos.y;
		rect.width = dim.w;
		rect.height = dim.h;
  }
};

struct CollisionCallback {
    function<void(Entity, Entity)> onCollision;

    CollisionCallback(function<void(Entity, Entity)> callback)
        : onCollision(callback) {}
};

struct Sprite {
	Texture2D tex;
	int z_index;
};
