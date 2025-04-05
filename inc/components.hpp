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
    float vx, vy;
};

struct Dimension
{
	float w, h;
};

struct Mass
{
	float v;
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
};
