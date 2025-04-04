#include "../inc/player.hpp"

Player init_player(ECS& ecs) {
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
	
  return Player{ playerId };
}

