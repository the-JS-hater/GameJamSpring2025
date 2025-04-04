#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/sparseSet.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/utils.hpp"


struct Player
{
	Entity id;
};

Player initPlayer(ECS& ecs)
{
	Texture2D playerTex = LoadTexture("resources/sprites/Spam.png");
	Entity playerId = ecs.createEntity();

  Position pos = { 
		static_cast<float>(rand() % GetScreenWidth()), 
		static_cast<float>(rand() % GetScreenHeight()) 
	};

  Velocity vel = { 0.0f, 0.0f };
  Dimension dim = { 32.0f, 32.0f };

	Collider collider;
  collider.update(pos, dim);
  
  ecs.positions.insert(playerId, pos);
  ecs.velocities.insert(playerId, vel);
  ecs.dimensions.insert(playerId, dim);
  ecs.colliders.insert(playerId, collider);
	ecs.sprites.insert(playerId, Sprite{playerTex});
	
	return Player {playerId};
}

int main()
{
	SetTraceLogLevel(LOG_WARNING);
  InitWindow(1280, 720, "Game");
	
	ECS ecs; 
	Player player = initPlayer(ecs);

	//genTestEntities(ecs, 1);

  Rectangle worldBounds = {
    0, 0, 
    static_cast<float>(GetScreenWidth()), 
    static_cast<float>(GetScreenHeight())
  };
  Quadtree quadtree(worldBounds);

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		/* UPDATE */
		if (IsKeyDown(KEY_W)) (*ecs.velocities.getComponent(player.id)).vy -= 1.0f;
		if (IsKeyDown(KEY_A)) (*ecs.velocities.getComponent(player.id)).vx -= 1.0f;
		if (IsKeyDown(KEY_S)) (*ecs.velocities.getComponent(player.id)).vy += 1.0f;
		if (IsKeyDown(KEY_D)) (*ecs.velocities.getComponent(player.id)).vx += 1.0f;

  	ecs.updateMovement();
  	//quadtree.update(ecs);
  	//auto vec = quadtree.getAllCollisions(ecs);
		//ecs.resolveCollisions(vec);
  	  
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	ecs.renderEntities();

		EndDrawing();
	}

	CloseWindow();
}
