#pragma once
#include <tuple>
#include <utility>
#include <array>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include "ECSCore.h"
#include "CompArray.h"
#include "ComponentAdmin.h"

template<typename... Components>
class ComponentView
{

public:
	ComponentView(CompSignature relevantSignature) : m_RelevantSignature(relevantSignature)
	{
		m_ComponentTable.reserve(MAX_ENTITIES);
		m_ContainedEntityIDs.reserve(MAX_ENTITIES);
		m_EntitiesToTableIndex.fill(-1);
		m_TableIndexToEntity.fill(-1);
	}

	const std::vector<std::tuple<Components&...>>& GetComponents()
	{
		return m_ComponentTable;
	}

	void Add(const EntityID& id, std::tuple<Components&...> tableEntry)
	{
		m_ComponentTable.push_back(tableEntry);
		m_ContainedEntityIDs.insert(id);

		m_EntitiesToTableIndex[id] = m_Size;
		m_TableIndexToEntity[m_Size] = id;

		m_Size++;
	}

	void Remove(const EntityID& id)
	{
		if (m_ComponentTable.size() == 0) { return; }

		if (m_EntitiesToTableIndex[id]==m_ComponentTable.size()-1)
		{
			m_ComponentTable.pop_back();
		}
		else
		{
			m_ComponentTable[m_EntitiesToTableIndex[id]] = std::move(m_ComponentTable.back());
			m_ComponentTable.pop_back();
			m_ContainedEntityIDs.erase(id);

			size_t indexOfRemovedEntity = m_EntitiesToTableIndex[id];
			size_t indexOfLastElement = m_Size - 1;
			EntityID entityOfLastElement = m_TableIndexToEntity[indexOfLastElement];

			m_EntitiesToTableIndex[entityOfLastElement] = indexOfRemovedEntity;
			m_TableIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

			m_EntitiesToTableIndex[id] = -1;
			m_TableIndexToEntity[indexOfLastElement] = -1;
		}

		--m_Size;

	}

	const CompSignature& GetRelevantSignature() { return m_RelevantSignature; }
	bool ContainsEntity(EntityID id) 
	{ 
		return  m_ContainedEntityIDs.size() != 0 && m_ContainedEntityIDs.find(id) != m_ContainedEntityIDs.end();
	}

private:
	size_t m_Size = 0;
	CompSignature m_RelevantSignature;
	std::unordered_set<EntityID> m_ContainedEntityIDs;
	std::array<size_t, MAX_ENTITIES> m_EntitiesToTableIndex;
	std::array<EntityID, MAX_ENTITIES> m_TableIndexToEntity;
	std::vector<std::tuple<Components&...>> m_ComponentTable;
};
