#include "../inc/ecs.hpp"


Entity ECS::createEntity()
{
  return nextEntity++;
}

void ECS::destroyEntity(Entity id)
{
  positions.remove(id);
  velocities.remove(id);
	dimensions.remove(id);
	colliders.remove(id);
	collisionCallbacks.remove(id);
}

void ECS::updateMovement()
{
  for (Entity id = 0; id < nextEntity; ++id) 
  {
    Position* pos = positions.getComponent(id);
    Velocity* vel = velocities.getComponent(id);

    if (!pos or !vel) continue; 

		pos->x += vel->vx * 1.0f;
    pos->y += vel->vy * 1.0f;
	}
}
	
void ECS::renderEntities()
{
  for (Entity id = 0; id < nextEntity; ++id) 
  {
    Position* pos = positions.getComponent(id);
    Sprite* sprite = sprites.getComponent(id);
    
    if (!pos || !sprite) continue;

    DrawTexture(
      sprite->tex, 
      static_cast<int>(pos->x), 
      static_cast<int>(pos->y), 
      WHITE
    );
  }
}

void ECS::resolveCollisions(vector<pair<Entity, Entity>> const& collisions) 
{
  for (const auto& [entityA, entityB] : collisions) 
  {
    Collider* colA = colliders.getComponent(entityA);
    Velocity* velA = velocities.getComponent(entityA);
		Mass* massA = masses.getComponent(entityA);
    CollisionCallback* callbackA = collisionCallbacks.getComponent(entityA);

    Collider* colB = colliders.getComponent(entityB);
    Velocity* velB = velocities.getComponent(entityB);
		Mass* massB = masses.getComponent(entityB);
    CollisionCallback* callbackB = collisionCallbacks.getComponent(entityB);
																	 
    if (!colA || !velA || !massA || !colB || !velB || !massB) continue;

    Vector2 posA = { colA->rect.x, colA->rect.y };
    Vector2 posB = { colB->rect.x, colB->rect.y };

    Vector2 velA_old = { velA->vx, velA->vy };
    Vector2 velB_old = { velB->vx, velB->vy };

    Vector2 relVel = Vector2Subtract(velA_old, velB_old);
    Vector2 collisionNormal = Vector2Subtract(posA, posB);
    float distSquared = Vector2LengthSqr(collisionNormal);

    if (distSquared == 0.0f) continue;

    collisionNormal = Vector2Scale(collisionNormal, 1.0f / sqrtf(distSquared));

    if (Vector2DotProduct(relVel, collisionNormal) >= 0.0f) continue; 

    float massFactor = (2.0f * massB->v) / (massA->v + massB->v);

    float impactSpeed = Vector2DotProduct(relVel, collisionNormal) * massFactor;
    Vector2 velocityChange = Vector2Scale(collisionNormal, impactSpeed);

    Vector2 velA_new = Vector2Subtract(velA_old, velocityChange);
    Vector2 velB_new = Vector2Add(velB_old, velocityChange);

    velA->vx = velA_new.x;
    velA->vy = velA_new.y;
    velB->vx = velB_new.x;
    velB->vy = velB_new.y;

    if (callbackA && callbackA->onCollision) {
        callbackA->onCollision(entityA, entityB);
    }
    if (callbackB && callbackB->onCollision) {
        callbackB->onCollision(entityB, entityA);
    }
  }
}
