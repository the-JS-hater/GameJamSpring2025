#include "../inc/quadtree.hpp"


QuadtreeNode::QuadtreeNode(const Rectangle& aabb, size_t lvl)
    : aabb(aabb), entityCount(0), lvl(lvl) 
{
  children.fill(nullptr);
}

void QuadtreeNode::subdivide() 
{
  float halfw=aabb.width / 2.0f;
  float halfh=aabb.height / 2.0f;
  children[0]=new QuadtreeNode({aabb.x,aabb.y,halfw,halfh},lvl+1); 
  children[1]=new QuadtreeNode({aabb.x+halfw,aabb.y,halfw,halfh},lvl+1); 
  children[2]=new QuadtreeNode({aabb.x,aabb.y+halfh,halfw,halfh},lvl+1); 
  children[3]=new QuadtreeNode({aabb.x + halfw,aabb.y+halfh,halfw,halfh},lvl+1);
}

void QuadtreeNode::insert(Entity id, Rectangle const& rect) 
{
  if (!CheckCollisionRecs(aabb, rect)) 
	{
    return; 
  }

  if (entityCount < MAX_ENTITIES) 
	{
    entities[entityCount++] = id; 
  } 
	else 
	{
    if (children[0] == nullptr) 
		{
      subdivide(); 
    }

    for (auto& child : children) 
		{
      child->insert(id, rect);
    }
  }
}

void QuadtreeNode::retrieve(vector<Entity>& result, Rectangle const& rect) 
{
  if (!CheckCollisionRecs(aabb, rect)) 
	{
    return; 
  }

  for (size_t i = 0; i < entityCount; ++i) 
	{
    result.push_back(entities[i]);
  }

  for (auto& child : children) 
	{
    if (child != nullptr) 
		{
      child->retrieve(result, rect);
    }
  }
}

void QuadtreeNode::clear() 
{
  for (auto& child : children) 
  {
    if (child != nullptr) 
    {
      child->clear(); 
      delete child;   
      child = nullptr;
    }
  }
  entityCount = 0; 
}

Quadtree::Quadtree(Rectangle const& aabb) 
{
  root = new QuadtreeNode(aabb, 0);
}

void Quadtree::insert(Entity id, const Collider& collider) 
{	
  root->insert(id, collider.rect);
}

void Quadtree::retrieve(vector<Entity>& result, Rectangle const& rect) 
{
  root->retrieve(result, rect);
}

void Quadtree::clear() 
{
  if (root != nullptr) 
  {
    root->clear(); 
  }
}	


void Quadtree::update(ECS& ecs)
{	
	clear();
  for (Entity id = 0; id < ecs.nextEntity; ++id) 
	{
		Collider* collider = ecs.colliders.getComponent(id);
		if (!collider) continue;

    Position* pos = ecs.positions.getComponent(id);
    Dimension* dim = ecs.dimensions.getComponent(id);

    if (!pos or !dim) continue; 
		
		collider->update(*pos, *dim);
		insert(id, *collider);
	}
}


vector<pair<Entity, Entity>> Quadtree::getAllCollisions(ECS& ecs) 
{
  vector<pair<Entity, Entity>> collisions;
	
  for (Entity id = 0; id < ecs.nextEntity; ++id) 
	{
    Collider* collider = ecs.colliders.getComponent(id);
    if (!collider) continue;

    vector<Entity> candidates;
    retrieve(candidates, collider->rect);

    for (Entity otherId : candidates) 
		{
      if (otherId <= id) continue;

      Collider const* otherCollider = ecs.colliders.getComponent(otherId);
      if (otherCollider and CheckCollisionRecs(collider->rect, otherCollider->rect)) 
			{
        collisions.emplace_back(id, otherId);
      }
    }
  }

  return collisions;
}
