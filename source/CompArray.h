#pragma once
#include <cassert>
#include <unordered_map>
#include <array>

#include "ECSCore.h"

class ICompArray
{
public:
	virtual ~ICompArray() = default;
	virtual void EntityDestroyed(EntityID entity) = 0;
};


template<typename T>
class CompArray : public ICompArray
{
public:
	void AddComponent(EntityID entity, T component)
	{
		assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");
		size_t newIndex = m_Size;
		m_EntityToIndexMap[entity] = newIndex;
		m_IndexToEntityMap[newIndex] = entity;
		m_ComponentArray[newIndex] = component;
		++m_Size;
	}

	void RemoveComponent(EntityID entity)
	{
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Removing non-existent component.");

		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
		size_t indexOfLastElement = m_Size - 1;
		m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

		EntityID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexMap.erase(entity);
		m_IndexToEntityMap.erase(indexOfLastElement);

		--m_Size;
	}

	T& GetComponent(EntityID entity)
	{
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Retrieving non-existent component.");
		return m_ComponentArray[m_EntityToIndexMap[entity]];
	}

	void EntityDestroyed(EntityID entity) override
	{
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
		{
			RemoveComponent(entity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> m_ComponentArray;
	std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
	std::unordered_map<size_t, EntityID> m_IndexToEntityMap;
	size_t m_Size;
};