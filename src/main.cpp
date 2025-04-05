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

  Position pos_1 {static_cast<float>(WINDOW_W)/2.0f, static_cast<float>(WINDOW_H)/4.0f,};
  Position pos_2 {pos_1.x, pos_1.y * 3};

	Texture2D gloveTex = LoadTexture("resources/sprites/Glove.png");
	Texture2D ratTex = LoadTexture("resources/sprites/Rat.png");
	Texture2D gloveTex2 = LoadTexture("resources/sprites/Glove2.png");
	Texture2D ratTex2 = LoadTexture("resources/sprites/Rat2.png");

	Player player_1 = init_player(ecs, pos_1, gloveTex, ratTex);
	Player player_2 = init_player(ecs, pos_2, gloveTex2, ratTex2);
		
	ElectricHex hex(20.0f, 15.0f, 5.0f, 2.0f, 50.0f, 30.0f); 

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
    player_1.input(ecs);
		player_1.update(ecs);
    player_2.input(ecs);
		player_2.update(ecs);

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
		player_1.drawArms(ecs);
		player_2.drawArms(ecs);
		  //
		  // printf(
		  //   "Player 1 (%lu %lu %lu)\n\tBody\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n\tLeft\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n\n\tRight\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n",
		  //   player_1.body, player_1.left, player_1.right,
		  //   (*ecs.masses.getComponent(player_1.body)).v,
		  //   (*ecs.accelerations.getComponent(player_1.body)).accX, (*ecs.accelerations.getComponent(player_1.body)).accY,
		  //   (*ecs.velocities.getComponent(player_1.body)).vx, (*ecs.velocities.getComponent(player_1.body)).vy,
		  //   (*ecs.masses.getComponent(player_1.left)).v,
		  //   (*ecs.accelerations.getComponent(player_1.left)).accX, (*ecs.accelerations.getComponent(player_1.left)).accY,
		  //   (*ecs.velocities.getComponent(player_1.left)).vx, (*ecs.velocities.getComponent(player_1.left)).vy,
		  //   (*ecs.masses.getComponent(player_1.right)).v,
		  //   (*ecs.accelerations.getComponent(player_1.right)).accX, (*ecs.accelerations.getComponent(player_1.right)).accY,
		  //   (*ecs.velocities.getComponent(player_1.right)).vx, (*ecs.velocities.getComponent(player_1.right)).vy
		  // );
		  //
		  // printf(
		  //   "Player 2 (%lu %lu %lu)\n\tBody\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n\tLeft\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n\n\tRight\n\t\t- mass: %f\n\t\t- acc: (%f, %f)\n\t\t- vel: (%f, %f)\n",
		  //   player_2.body, player_2.left, player_2.right,
		  //   (*ecs.masses.getComponent(player_2.body)).v,
		  //   (*ecs.accelerations.getComponent(player_2.body)).accX, (*ecs.accelerations.getComponent(player_2.body)).accY,
		  //   (*ecs.velocities.getComponent(player_2.body)).vx, (*ecs.velocities.getComponent(player_2.body)).vy,
		  //   (*ecs.masses.getComponent(player_2.left)).v,
		  //   (*ecs.accelerations.getComponent(player_2.left)).accX, (*ecs.accelerations.getComponent(player_2.left)).accY,
		  //   (*ecs.velocities.getComponent(player_2.left)).vx, (*ecs.velocities.getComponent(player_2.left)).vy,
		  //   (*ecs.masses.getComponent(player_2.right)).v,
		  //   (*ecs.accelerations.getComponent(player_2.right)).accX, (*ecs.accelerations.getComponent(player_2.right)).accY,
		  //   (*ecs.velocities.getComponent(player_2.right)).vx, (*ecs.velocities.getComponent(player_2.right)).vy
		  // );
		  //
		ecs.renderEntities();

		drawCollisionDetection(vec, ecs);

		EndDrawing();
	}

	CloseWindow();
}
