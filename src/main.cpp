#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/player.hpp"
#include "../inc/utils.hpp"
#include "../inc/hex.hpp"
#include <stdio.h>
#include <cstdlib>

#define WINDOW_W 1280
#define WINDOW_H 720 
	
int main()
{
	SetTraceLogLevel(LOG_WARNING);
  InitWindow(WINDOW_W, WINDOW_H, "Game");
	srand(time(NULL));
	
	ECS ecs; 
	Player player = init_player(ecs);
		
	ElectricHex hex(20.0f, 15.0f, 5.0f, 2.0f, 50.0f, 30.0f); 

  Rectangle worldBounds = {
    0, 0, 
    static_cast<float>(GetScreenWidth()), 
    static_cast<float>(GetScreenHeight())
  };
  Quadtree quadtree(worldBounds);

	genTestEntities(ecs, 10); 

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		/* UPDATE */
    player.input(ecs);
		player.update(ecs);

    game_pad();

    ecs.updateVelocities();
  	ecs.updateMovement();

  	quadtree.update(ecs);
  	auto vec = quadtree.getAllCollisions(ecs);

		ecs.resolveCollisions(vec);
		hex.checkHexBounds(ecs);
 		
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	hex.draw();
		ecs.renderEntities();


		EndDrawing();
	}

	CloseWindow();
}
