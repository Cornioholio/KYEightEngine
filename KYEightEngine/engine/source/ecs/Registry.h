#pragma once
#include "Entity.h"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <utility>

#include <iostream>
/*
Little mental map for my ecs:

Registry
	|
    V
Entities -> 1, 2, 3, 4, etc..
	|
	V
Storage -> Transform3DComponent -> NameComponent -> VelocityComponent
					|					|					|
					V					V					V
			1 -> Transform			1 -> "Player"		2 -> Velocity
			2 -> Transform
*/
namespace KYEight 
{
	class Registry 
	{
	public:
		// Create new entity and return id
		Entity CreateEntity();

		// Destroy entity and remove components
		void DestroyEntity(Entity entity);

		// Check if entity exists
		bool IsValid(Entity entity) const;

		template<typename T, typename... Args>
		T& AddComponent(Entity entity, Args&&... args)
		{
			// Get the storage specific to component type T.
			auto& storage = GetComponentStorage<T>();

			// Add the component to the actual unordered_map.
			auto [it, inserted] = storage.components.emplace(entity, T(std::forward<Args>(args)...));

			return it->second;
		}

		template<typename T>
		void RemoveComponent(Entity entity) 
		{
			// Look for the storage belonging to component type T
			auto it = componentStorage_.find(std::type_index(typeid(T)));

			// If no storage exists return early
			if (it == componentStorage_.end()) 
			{
				return; 
			}

			// Store does contain T at this point, cast storage to its real type
			auto* storage = static_cast<ComponentStorage<T>*>(it->second.get());

			// Remove the commponent in storage
			storage->components.erase(entity);	
		}
		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get the storage for T, then find entity inside storage
			
			return GetComponentStorage<T>().components.at(entity);
		}
		template<typename T>
		const T& GetComponent(Entity entity) const
		{
			auto it = componentStorage_.find(std::type_index(typeid(T)));

			if (it == componentStorage_.end()) 
			{
				// use logger later
				std::cout << "Component storage does not exist\n";
			}

			const auto* storage = static_cast<const ComponentStorage<T>*>(it->second.get());

			return storage->components.at(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity) const
		{
			// Find the storage for T 
			auto it = componentStorage_.find(std::type_index(typeid(T)));

			// If no storage exists the entity cannot have the component, so return early
			if (it == componentStorage_.end())
			{
				return false;
			}

			// Storage exists, cast to real type
			auto* storage = static_cast<ComponentStorage<T>*>(it->second.get());

			// Check entity storage for the component and return result
			return storage->components.contains(entity);
		}

		const std::unordered_set<Entity>& GetActiveEntities() const 
		{
			return activeEntities_;
		}
	private:

		// Component storage interface, used for type erasure
		struct IComponentStorage 
		{
			// Virtual destructor to ensure proper cleanup of derived classes
			virtual ~IComponentStorage() = default;

			// Every storage must know to remove a component
			virtual void Remove(Entity entity) = 0;
		};

		template<typename T> 
		struct ComponentStorage : IComponentStorage
		{
			// Map of entity ID's to their components
			std::unordered_map<Entity, T> components;

			// Does what it says on the tin
			void Remove(Entity entity) override 
			{
				components.erase(entity);
			}
		};

		template<typename T>
		ComponentStorage<T>& GetComponentStorage()
		{
			// Convert type T to a value that can be used as a key in component storage map
			auto type = std::type_index(typeid(T));

			// Search for an existing storage for this component type
			auto it = componentStorage_.find(type);

			// If no such storage exists yet
			if (it == componentStorage_.end())
			{
				// Create a new storage for T
				auto storage = std::make_unique<ComponentStorage<T>>();

				// Insert it into the generic storage map, newIt points to newly inserted storage
				auto [newIt, inserted] = componentStorage_.emplace(type, std::move(storage));

				// Use new iterator now
				it = newIt;
			}

			// Cast back to real type and return reference to storage
			return *static_cast<ComponentStorage<T>*>(it->second.get());
		}
		// All component storage, the key identifies the component type, the value points to the storage for the type
		std::unordered_map <std::type_index, std::unique_ptr<IComponentStorage>> componentStorage_;

		// Every new entity that exists, and the id we give to the next one we create
		std::unordered_set<Entity> activeEntities_;
		Entity nextEntityId_ = 1;

	};
}