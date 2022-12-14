#pragma once
#include <tuple>
#include <utility>
#include <array>
#include <variant>
#include <unordered_set>
#include <memory>
#include "ECSCore.h"
#include "CompArray.h"
#include "ComponentAdmin.h"

template<typename... Components>
class ComponentView
{

public:
	ComponentView(
		const std::unordered_set<EntityID>& aliveEntites,
		const std::array<CompSignature, MAX_ENTITIES>& signatures,
		CompSignature relevantSignature,
		const std::unique_ptr<ComponentAdmin>& compAdmin) :
		m_AliveEntities(aliveEntites),
		m_Signatures(signatures),
		m_RelevantSignature(relevantSignature),
		m_CompAdmin(compAdmin)
	{}

	std::vector<std::tuple<Components&...>> GetComponents()
	{
		std::vector<std::tuple<Components&...>> table;
			for (EntityID id : m_AliveEntities)
			{
				if ((m_Signatures[id] & m_RelevantSignature) == m_RelevantSignature)
				{			
					table.push_back(std::tie(*&(m_CompAdmin->GetComponent<Components>(id))...));
				}
			}
			return table;
	}

private:

	const std::unique_ptr<ComponentAdmin>& m_CompAdmin;

	CompSignature m_RelevantSignature;
	const std::array<CompSignature, MAX_ENTITIES>& m_Signatures;
	const std::unordered_set<EntityID>& m_AliveEntities;
};
