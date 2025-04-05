#pragma once 


#include <memory>
#include <iterator>
#include <raylib.h>
#include "../inc/ecs.hpp"

struct Triangle
{
	Vector2 a;
	Vector2 b;
	Vector2 c;
};


struct ElectricHex
{
	Rectangle outerBox;
	Triangle corners[4];

	ElectricHex(
		float scale, float offset,
		float x, float y,
		float w, float h
	);

	void draw();

	bool inBounds(Rectangle aabb);

	void resolveCollision(ECS& ecs, Entity id);
};
