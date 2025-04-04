#include "../inc/sparseSet.hpp"


template<typename T>
void SparseSet<T>::insert(Entity id, T component)
{
	if (entityToIndex.find(id) == entityToIndex.end()) 
	{
    size_t idx = dense.size();
    dense.push_back(component);
    entityToIndex[id] = idx;
    idxToEntity[idx] = id;
  } 
	else 
	{
    dense[entityToIndex[id]] = component;
  }
}

template<typename T>
void SparseSet<T>::remove(Entity id)
{
  auto it = entityToIndex.find(id);
  if (it != entityToIndex.end()) 
	{
    size_t idx = it->second;
    size_t lastIdx = dense.size() - 1;

    if (idx != lastIdx) 
		{
      dense[idx] = dense[lastIdx];
      size_t lastEntity = idxToEntity[lastIdx];
      entityToIndex[lastEntity] = idx;
      idxToEntity[idx] = lastEntity;
    }

    dense.pop_back();
    entityToIndex.erase(it);
    idxToEntity.erase(lastIdx);
  }
}

template<typename T>
T* SparseSet<T>::getComponent(Entity id)
{
  auto it = entityToIndex.find(id);
  if (it != entityToIndex.end()) 
	{
    return &dense[it->second];
  }
  return nullptr;
}

template<typename T>
size_t SparseSet<T>::size() const 
{
	return dense.size();
}

