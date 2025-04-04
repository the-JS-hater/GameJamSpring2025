#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/player.hpp"

int main()
{
	SetTraceLogLevel(LOG_WARNING);
  InitWindow(1280, 720, "Game");
	
	ECS ecs; 
	Player player = init_player(ecs);

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
  	ecs.updateMovement();
    player.input(ecs);
		// 	quadtree.update(ecs);
		// 	auto vec = quadtree.getAllCollisions(ecs);
		// ecs.resolveCollisions(vec);
  	  
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	ecs.renderEntities();

		EndDrawing();
	}

	CloseWindow();
}
