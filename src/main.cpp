#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/player.hpp"
#include "../inc/utils.hpp"
#include "../inc/hex.hpp"
#include <stdio.h>

#define WINDOW_W 1280
#define WINDOW_H 720 
	

int main()
{
	SetTraceLogLevel(LOG_WARNING);
  InitWindow(WINDOW_W, WINDOW_H, "Game");
	
	ECS ecs; 
	Player player = init_player(ecs);
		
	ElectricHex hex(20.0f, 15.0f, 5.0f, 2.0f, 50.0f, 30.0f); 

	genTestEntities(ecs, 100);

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
    player.input(ecs);
  	ecs.updateMovement();
  	quadtree.update(ecs);
  	auto vec = quadtree.getAllCollisions(ecs);
		ecs.resolveCollisions(vec);
 		
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	hex.draw();
		ecs.renderEntities();


		EndDrawing();
	}

	CloseWindow();
}
