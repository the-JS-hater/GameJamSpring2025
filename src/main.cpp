#include <ctime>
#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/player.hpp"
#include "../inc/utils.hpp"
#include "../inc/hex.hpp"
#include "../inc/audioManager.hpp"
#include <stdio.h>
#include <cstdlib>

#define WINDOW_W 1280
#define WINDOW_H 720 
	
int main()
{
	SetTraceLogLevel(LOG_WARNING);
  InitWindow(WINDOW_W, WINDOW_H, "Game");
	InitAudioDevice();                                     
	srand(time(NULL));
	initSounds();

	
	ECS ecs; 

  Position pos_1 {static_cast<float>(WINDOW_W)/2.0f, static_cast<float>(WINDOW_H)/4.0f,};
  Position pos_2 {pos_1.x, pos_1.y * 3};

	Texture2D gloveTex = LoadTexture("resources/sprites/Glove.png");
	Texture2D ratTex = LoadTexture("resources/sprites/Rat.png");
	Texture2D gloveTex2 = LoadTexture("resources/sprites/Glove2.png");
	Texture2D ratTex2 = LoadTexture("resources/sprites/Rat2.png");
	Texture2D crowd1 = LoadTexture("resources/sprites/Crowd1.png");
	Texture2D crowd2 = LoadTexture("resources/sprites/Crowd2.png");

	Player player_1 = init_player(ecs, pos_1, gloveTex, ratTex);
	Player player_2 = init_player(ecs, pos_2, gloveTex2, ratTex2);
		
	ElectricHex hex(20.0f, 5.0f, 10.0f, 3.0f, 45.0f, 30.0f); 

  Rectangle worldBounds = {
    0, 0, 
    static_cast<float>(GetScreenWidth()), 
    static_cast<float>(GetScreenHeight())
  };
  Quadtree quadtree(worldBounds);

  ToggleFullscreen();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		/* UPDATE */
    player_1.input(ecs);
		player_1.update(ecs);
    player_2.input(ecs);
		player_2.update(ecs);

    ecs.updateVelocities();
  	ecs.updateMovement();

  	quadtree.update(ecs);
  	auto vec = quadtree.getAllCollisions(ecs);

		ecs.resolveCollisions(vec);
		hex.checkHexBounds(ecs, player_1, player_2);
 		
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	hex.draw();
		player_1.drawArms(ecs);
		player_2.drawArms(ecs);
    player_1.draw_score(1, 20);
    player_2.draw_score(2, 50);
		ecs.renderEntities();
			
		DrawTextureEx(
			crowd1, 
			{0.0f, GetScreenHeight() / 2 - crowd1.height}, 
			0.0f, 
			2.0f, 
			WHITE
		);
		DrawTextureEx(
			crowd2, 
			{
				GetScreenWidth() - 2.0f * crowd2.width, 
				GetScreenHeight() / 2 - crowd2.height	
			}, 
			0.0f, 
			2.0f, 
			WHITE
		);

		//drawCollisionDetection(vec, ecs);

		EndDrawing();
	}

	CloseWindow();
}
