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

	Entity& CreateEntity(Game* activeGame)
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
		EntityID id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		++m_LivingEntityCount;
		m_Entities[id] = Entity(id, activeGame);
		return m_Entities[id];
	}

	void DestroyEntity(EntityID entityID)
	{
		assert(entityID < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entityID].reset();
		m_Entities[entityID].SetAlive(false);
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

	uint32_t GetMatchingSignatureCount(const CompSignature& signature)
	{
		uint32_t matchingCount = 0;
		uint32_t nonActiveCount = 0;
		for (size_t i = 0; i < MAX_ENTITIES; i++)
		{
			if (nonActiveCount >= 20) { break; }

			Entity& entity = m_Entities[i];
			if (entity.IsAlive()) 
			{
				nonActiveCount++;
				continue;
			}

			const CompSignature& entitySig = m_Signatures[i];
			if ((entitySig & signature) == signature) 
			{
				matchingCount++;
			}
		}
		return matchingCount;
	}

private:
	std::queue<EntityID> m_AvailableEntities{};

	//TODO maybe move signature into entity class
	std::array<CompSignature, MAX_ENTITIES> m_Signatures{};
	std::array<Entity, MAX_ENTITIES> m_Entities{};

	uint32_t m_LivingEntityCount{};
};
