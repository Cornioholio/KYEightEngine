#include "Registry.h"

namespace KYEight
{
	Entity Registry::CreateEntity() 
	{
		// Give the new entity a unique ID and add it to the active entities set
		Entity entity = nextEntityId_++;

		activeEntities_.insert(entity);

		return entity;
	}
	void Registry::DestroyEntity(Entity entity)
	{
		// Try remove entity from active entities set, if it doesnt exist return early
		if (!activeEntities_.contains(entity)) 
		{
			return;
		}

		// Cleanup all components associated with the entity
		for (auto& [type, storage] : componentStorage_) 
		{
			storage->Remove(entity);
		}

		activeEntities_.erase(entity);
	}

	bool Registry::IsValid(Entity entity) const 
	{
		// Returns true if this entity exists in set
		return activeEntities_.contains(entity);
	}
}
