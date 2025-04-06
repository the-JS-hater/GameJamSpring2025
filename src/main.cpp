#include <ctime>
#include <raylib.h>
#include "../inc/ecs.hpp"
#include "../inc/quadtree.hpp"
#include "../inc/player.hpp"
#include "../inc/utils.hpp"
#include "../inc/hex.hpp"
#include "../inc/audioManager.hpp"
#include "../inc/victory_screen.hpp"
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

	Texture2D gloveTex 		= LoadTexture("resources/sprites/Glove.png");
	Texture2D ratTex 			= LoadTexture("resources/sprites/Rat.png");
	Texture2D gloveTex2 	= LoadTexture("resources/sprites/Glove2.png");
	Texture2D ratTex2 		= LoadTexture("resources/sprites/Rat2.png");
	Texture2D crowd 			= LoadTexture("resources/sprites/animCrowd.png"); 
	Texture2D crowd2 			= LoadTexture("resources/sprites/animCrowd2.png"); 
	
	Player player_1 = init_player(ecs, pos_1, gloveTex, ratTex);
	Player player_2 = init_player(ecs, pos_2, gloveTex2, ratTex2);
  VictoryScreen screen(WINDOW_W, WINDOW_H);
  
	ElectricHex hex(20.0f, 15.0f, 8.0f, 2.0f, 47.0f, 30.0f); 

  Rectangle worldBounds = {
    0, 0, 
    static_cast<float>(GetScreenWidth()), 
    static_cast<float>(GetScreenHeight())
  };
  Quadtree quadtree(worldBounds);

  bool is_win = false;

  ToggleFullscreen();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
    if (is_win) {
      BeginDrawing();
  	  ClearBackground(WHITE);
      screen.print_screen(player_1, player_2);
      screen.input(player_1, player_2, is_win);
      EndDrawing();
      continue;
    }

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
		hex.checkHexBounds(ecs, player_1, player_2, is_win);
 		
		playRandomCrowdAudio();
		
		/* RENDERING */
  	
  	BeginDrawing();
  	ClearBackground(WHITE);
  	
  	hex.draw();
		player_1.drawArms(ecs);
		player_2.drawArms(ecs);
    player_1.draw_score(1, 20);
    player_2.draw_score(2, 50);
		ecs.renderEntities();
		
		drawCrowds(crowd, crowd2);

		EndDrawing();
	}

	CloseWindow();
}
