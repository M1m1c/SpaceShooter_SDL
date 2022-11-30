#pragma once
#include <cstdint>
#include <queue>
#include <array>
#include <cassert>

#include "ECSCore.h"
#include "Entity.h"

class EntityAdmin
{
public:
	EntityAdmin()
	{
		for (EntityID entityID = 0; entityID < MAX_ENTITIES; ++entityID)
		{
			m_AvailableEntities.push(entityID);
			m_Entities[entityID] = Entity(entityID);
		}
	}

	Entity& CreateEntity(Scene* activeScene)
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
		EntityID id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		++m_LivingEntityCount;
		m_Entities[id] = Entity(id, activeScene);
		return m_Entities[id];
	}

	void DestroyEntity(EntityID entityID)
	{
		assert(entityID < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entityID].reset();
		m_Entities[entityID] = Entity(entityID, nullptr);
		m_AvailableEntities.push(entityID);
		--m_LivingEntityCount;
	}

	void SetSignature(EntityID entity, CompSignature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entity] = signature;
	}

	CompSignature GetSignature(EntityID entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return m_Signatures[entity];
	}

private:
	std::queue<EntityID> m_AvailableEntities{};

	std::array<CompSignature, MAX_ENTITIES> m_Signatures{};
	std::array<Entity, MAX_ENTITIES> m_Entities{};

	uint32_t m_LivingEntityCount{};
};
