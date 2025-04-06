#include "../inc/ecs.hpp"
#include <cstdio>

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
  accelerations.remove(id);
  masses.remove(id);
	collisionCallbacks.remove(id);
}

void ECS::updateVelocities()
{
  for (Entity id = 0; id < nextEntity; ++id) 
	{
    Acceleration* acc = accelerations.getComponent(id);
    Velocity* vel = velocities.getComponent(id);

		if (!acc || !vel) continue;
			
		if (acc->accX == 0.0f) vel->vx *= acc->retarding_factor;
		if (acc->accY == 0.0f) vel->vy *= acc->retarding_factor;
		// so slow you might as well stop
		if (vel->vx > 0.0f and vel->vx < 0.1f) vel->vx = 0.0f;
		if (vel->vy > 0.0f and vel->vy < 0.1f) vel->vy = 0.0f;
		if (vel->vx < 0.0f and vel->vx > -0.1f) vel->vx = 0.0f;
		if (vel->vy < 0.0f and vel->vy > -0.1f) vel->vy = 0.0f;
		// I am sped!
    vel->vx += acc->accX;
		vel->vy += acc->accY;

		// Fortkörningsböter!
		if (vel->vx > vel->max_v) vel->vx *= 	0.5f;
		if (vel->vy > vel->max_v) vel->vy *= 	0.5f;
		if (vel->vx < -vel->max_v) vel->vx *=	0.5f;
		if (vel->vy < -vel->max_v) vel->vy *=	0.5f;
	}
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

    Collider* colB = colliders.getComponent(entityB);
    Velocity* velB = velocities.getComponent(entityB);
		Mass* massB = masses.getComponent(entityB);

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

    float massFactorA = (2.0f * massB->v) / (massA->v + massB->v);
    float massFactorB = (2.0f * massA->v) / (massA->v + massB->v);

    float impactSpeedA = Vector2DotProduct(relVel, collisionNormal) * massFactorA;
    float impactSpeedB = Vector2DotProduct(relVel, collisionNormal) * massFactorB;

    Vector2 velocityChangeA = Vector2Scale(collisionNormal, impactSpeedA * 3);
    Vector2 velocityChangeB = Vector2Scale(collisionNormal, impactSpeedB * 3);

    Vector2 velA_new = Vector2Subtract(velA_old, velocityChangeA);
    Vector2 velB_new = Vector2Add(velB_old, velocityChangeB);
	
		if (massA->v == 250.0f xor massB->v == 250.0f) playRandomSmack();

    velA->vx = velA_new.x;
    velA->vy = velA_new.y;
    velB->vx = velB_new.x;
    velB->vy = velB_new.y;
  }
}
