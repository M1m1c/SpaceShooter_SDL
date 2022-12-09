#pragma once
#include <tuple>
#include <utility>
#include <array>
#include <variant>
#include <unordered_set>
#include "ECSCore.h"
#include "CompArray.h"

template<typename... Types>
using DataTable = std::vector<std::tuple<Types&...>>;



template<typename... Components>
class ComponentView
{

public:
	ComponentView(
		const std::unordered_set<EntityID>& aliveEntites,
		const std::array<CompSignature, MAX_ENTITIES>& signatures,
		CompSignature relevantSignature,
		CompArray<Components>&... arrays) :
		m_AliveEntities(aliveEntites),
		m_Signatures(signatures),
		m_RelevantSignature(relevantSignature),
		m_Arrays{ arrays... }
	{}

	DataTable<std::tuple<Components&...>> Get() {
		//TODO iterate over all alive entities, 
		//check the m_signatures if the current entity ID has m_relevant signature
		// then get refernces/ptrs to components from arrays and push them to the return array.
		DataTable<std::tuple<Components&...>> table;
		for (EntityID id : m_AliveEntities)
		{
			if ((m_Signatures[id] & m_RelevantSignature) == m_RelevantSignature)
			{
				std::tuple<Components&...> tuple;

				std::apply([&tuple, &id](auto&... arrays)
					{
						std::tie(std::get<Components&>(tuple)...) = std::make_tuple(arrays[id]...);
					}, 
					m_Arrays);

				table.push_back(tuple);
			}
		}
		return table;

	}

private:
	// Store references to the arrays of components.
	std::tuple<CompArray<Components>&...> m_Arrays;

	CompSignature m_RelevantSignature;
	const std::array<CompSignature, MAX_ENTITIES>& m_Signatures;
	const std::unordered_set<EntityID>& m_AliveEntities;
};
