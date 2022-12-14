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
	CompArray() : m_Size(0)
	{
		m_ComponentArray.fill(T());
		m_EntityToIndexArray.fill(-1);
		m_IndexToEntityArray.fill(-1);
	}

	void AddComponent(EntityID entity, T component)
	{
		assert(m_EntityToIndexArray[entity] == -1 && "Component added to same entity more than once.");
		size_t newIndex = m_Size;
		m_EntityToIndexArray[entity] = newIndex;
		m_IndexToEntityArray[newIndex] = entity;
		m_ComponentArray[newIndex] = component;
		++m_Size;
	}

	void RemoveComponent(EntityID entity)
	{
		assert(m_EntityToIndexArray[entity] != -1 && "Removing non-existent component.");

		size_t indexOfRemovedEntity = m_EntityToIndexArray[entity];
		size_t indexOfLastElement = m_Size - 1;
		m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

		EntityID entityOfLastElement = m_IndexToEntityArray[indexOfLastElement];
		m_EntityToIndexArray[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityArray[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexArray[entity] = -1;
		m_IndexToEntityArray[indexOfLastElement] = -1;

		--m_Size;
	}

	T& GetComponent(EntityID entity)
	{
		assert(entity < m_EntityToIndexArray.size() && "Retrieving non-existent component.");
		return m_ComponentArray[m_EntityToIndexArray[entity]];
	}

	void EntityDestroyed(EntityID entity) override
	{
		if (entity < m_EntityToIndexArray.size() && m_EntityToIndexArray[entity] != -1)
		{
			RemoveComponent(entity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> m_ComponentArray;
	std::array<size_t, MAX_ENTITIES> m_EntityToIndexArray;
	std::array<EntityID, MAX_ENTITIES>  m_IndexToEntityArray;
	size_t m_Size;
};