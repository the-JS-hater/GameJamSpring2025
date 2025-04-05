#pragma once

#include "components.hpp"
#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

using Entity = size_t;

template<typename T>
struct SparseSet
{
  vector<T> dense;
  unordered_map<Entity, size_t> entityToIndex;
  unordered_map<size_t, Entity> idxToEntity;

  void insert(Entity, T);

  void remove(Entity);

  T* getComponent(Entity);
  void setComponent(Entity, T&);
	
  size_t size() const;
};

template struct SparseSet<Position>; 
template struct SparseSet<Velocity>;
template struct SparseSet<Dimension>;
template struct SparseSet<Collider>;
template struct SparseSet<CollisionCallback>;
template struct SparseSet<Sprite>;
template struct SparseSet<Mass>;
template struct SparseSet<Acceleration>;
