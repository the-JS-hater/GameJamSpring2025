#pragma once

#include <stdio.h>
#include <raylib.h>
#include <cstdlib>


void drawCrowds(Texture2D crowd, Texture2D crowd2)
{
	Rectangle source {
		0.0f,
		(crowd.height / 3.0f) * (rand()%3),
		crowd.width,
		crowd.height / 3.0f 
	};

	DrawTexturePro(
		crowd,
		source,
		{
			10.0f, 
			GetScreenHeight() / 3, 
			2.0f * crowd.width, 
			2.0f * crowd.height / 3.0f
		}, 
		{0.0f, 0.0f},
		0.0f,
		WHITE
	);

	source.y = (crowd.height / 3.0f) * (rand()%3);
	
	DrawTexturePro(
		crowd2,
		source,
		{
			GetScreenWidth() * 0.88, 
			GetScreenHeight() / 3, 
			2.0f * crowd.width, 
			2.0f * crowd.height / 3.0f
		}, 
		{0.0f, 0.0f},
		0.0f,
		WHITE
	);
}

void genTestEntities(ECS& ecs, size_t count) 
{
	Texture2D tex = LoadTexture("resources/sprites/Spam.png");
  for (size_t i = 0; i < count; ++i) {
    Entity entity = ecs.createEntity();
    
    Position pos = { 
			static_cast<float>(rand() % GetScreenWidth()), 
			static_cast<float>(rand() % GetScreenHeight()) 
		};

    Acceleration acc = { 
			static_cast<float>((rand() % 2) - 0), 
			static_cast<float>((rand() % 2) - 0) 
		};
		
		Velocity vel = {
			1.0f,
			1.0f,
      2.0f
		};

    Dimension dim = { 
			32.0f,
			32.0f
		};

		Mass mass = {
			1000.0f
		};
    
		Collider collider;
    collider.update(pos, dim);
    
    ecs.positions.insert(entity, pos);
    ecs.velocities.insert(entity, vel);
		ecs.accelerations.insert(entity, acc);
    ecs.dimensions.insert(entity, dim);
    ecs.colliders.insert(entity, collider);
		ecs.masses.insert(entity, mass);
		ecs.sprites.insert(entity, Sprite{tex});
	}
}

void drawCollisionDetection(vector<pair<Entity, Entity>> vec, ECS& ecs)
{
	for (auto [e1, e2] : vec)
	{
		auto box1 = ecs.colliders.getComponent(e1)->rect;
		auto box2 = ecs.colliders.getComponent(e2)->rect;

		DrawRectangleRec(box1, RED);
		DrawRectangleRec(box2, RED);
	}
}
