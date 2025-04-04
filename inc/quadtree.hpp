#pragma once

#include "entityType.hpp"
#include "components.hpp"
#include "ecs.hpp"
#include <raylib.h>
#include <array>
#include <utility>
#include <vector>

#define MAX_ENTITIES 5
#define MAX_LEVELS 32  

using std::array;
using std::vector;
using std::pair;

struct QuadtreeNode 
{
  Rectangle aabb; 
  size_t entityCount; 
  size_t lvl; 
  array<Entity, MAX_ENTITIES> entities; 
  array<QuadtreeNode*, 4> children; 

  QuadtreeNode(Rectangle const&, size_t);

  void subdivide();

  void insert(Entity, Rectangle const&);

  void retrieve(vector<Entity>&, Rectangle const&); 
	
	void clear();
};

struct Quadtree
{
  vector<Collider> colliders; 
  QuadtreeNode* root;

  Quadtree(Rectangle const&);

  void insert(Entity, Collider const&);

  void retrieve(vector<Entity>&, Rectangle const&);

	vector<pair<Entity, Entity>> getAllCollisions(ECS&); 
	
	void update(ECS&);
	
	void clear();
};

