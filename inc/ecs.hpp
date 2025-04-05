#pragma once

#include "sparseSet.hpp"
#include "components.hpp"
#include "entityType.hpp"
#include <raylib.h>
#include <raymath.h>
#include <utility>

using std::vector;
using std::pair;

struct ECS 
{
  Entity nextEntity = 0;
  SparseSet<Position> positions;
  SparseSet<Velocity> velocities;
  SparseSet<Dimension> dimensions;
  SparseSet<Collider> colliders;
  SparseSet<CollisionCallback> collisionCallbacks;
	SparseSet<Sprite> sprites;
	SparseSet<Mass> masses;

  Entity createEntity();

  void destroyEntity(Entity);

  void updateMovement();

	void renderEntities();

	void resolveCollisions(vector<pair<Entity, Entity>> const&); 
};
