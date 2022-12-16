#pragma once
#include "ECSCore.h"
#include "../containers/FixedQueue.h"
#include "../containers/FixedSizeSet.h"
#include <cstdint>
#include <array>
#include <cassert>
#include <unordered_set>



class EntityAdmin
{
public:
	EntityAdmin()
	{
		for (EntityID entityID = 0; entityID < MAX_ENTITIES; ++entityID)
		{
			m_AvailableEntities.push(entityID);
		}
	}

	const EntityID& CreateEntity()
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
		auto& id = m_AvailableEntities.front();
		m_InUseEntityIDs.insert(id);
		m_AvailableEntities.pop();
		++m_LivingEntityCount;
		return id;
	}

	void DestroyEntity(EntityID entityID)
	{
		assert(entityID < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entityID].reset();
		m_AvailableEntities.push(entityID);
		m_InUseEntityIDs.erase(entityID);
		--m_LivingEntityCount;
	}

	void SetSignature(EntityID entity, CompSignature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		m_Signatures[entity] = signature;
	}

	
	uint32_t GetMatchingSignatureCount(const CompSignature& signature)
	{
		uint32_t matchingCount = 0;
		uint32_t nonActiveCount = 0;
		for (size_t i = 0; i < MAX_ENTITIES; i++)
		{
			
			if(!IsEntityActive(i))
			{
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

	CompSignature GetSignature(EntityID entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return m_Signatures[entity];
	}

	uint32_t GetLivingEntitiesCount()
	{
		return m_LivingEntityCount;
	}

	const FixedSizeSet<EntityID, MAX_ENTITIES>& GetActiveEntities()
	{
		return  m_InUseEntityIDs;
	}

	bool IsEntityActive(EntityID id)
	{
		return m_InUseEntityIDs.count(id) > 0;
	}


	const std::array<CompSignature, MAX_ENTITIES>& GetSignatures() 
	{
		return m_Signatures;
	}

private:
	FixedQueue<EntityID,MAX_ENTITIES> m_AvailableEntities{};
	FixedSizeSet<EntityID,MAX_ENTITIES> m_InUseEntityIDs;
	std::array<CompSignature, MAX_ENTITIES> m_Signatures{};
	uint32_t m_LivingEntityCount{};
};
