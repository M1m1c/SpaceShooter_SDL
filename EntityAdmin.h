#pragma once
#include <cstdint>
#include <queue>
#include <array>
#include <cassert>

#include "ECSCore.h"

class EntityAdmin
{
public:
	EntityAdmin()
	{
		for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	EntityID CreateEntity()
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
		EntityID id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		++m_LivingEntityCount;
		return id;
	}

	void DestroyEntity(EntityID entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entity].reset();
		m_AvailableEntities.push(entity);
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

	uint32_t m_LivingEntityCount{};
};
