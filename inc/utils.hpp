#pragma once


void genTestEntities(ECS& ecs, size_t count) 
{
	Texture2D tex = LoadTexture("resources/sprites/Spam.png");

  for (size_t i = 0; i < count; ++i) {
    Entity entity = ecs.createEntity();
    
    Position pos = { 
			static_cast<float>(rand() % GetScreenWidth()), 
			static_cast<float>(rand() % GetScreenHeight()) 
		};

    Velocity vel = { 
			static_cast<float>((rand() % 5) - 2), 
			static_cast<float>((rand() % 5) - 2) 
		};
    Dimension dim = { 
			32.0f,
			32.0f
		};
    
		Collider collider;
    collider.update(pos, dim);
    
    ecs.positions.insert(entity, pos);
    ecs.velocities.insert(entity, vel);
    ecs.dimensions.insert(entity, dim);
    ecs.colliders.insert(entity, collider);
		
		//CollisionCallback testCallback([&ecs](Entity this_id, Entity other_id){
		//	ecs.destroyEntity(other_id);
		//});
		//ecs.collisionCallbacks.insert(entity, testCallback);
 		
		ecs.sprites.insert(entity, Sprite{tex});
	}
}
