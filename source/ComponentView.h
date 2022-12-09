#pragma once
#include <tuple>
#include <utility>
#include <array>
#include <variant>
#include <unordered_set>
#include <memory>
#include "ECSCore.h"
#include "CompArray.h"

template<typename... Types>
using DataTable = std::vector<std::tuple<Types...>>;



template<typename... Components>
class ComponentView
{

public:
	ComponentView(
		const std::unordered_set<EntityID>& aliveEntites,
		const std::array<CompSignature, MAX_ENTITIES>& signatures,
		CompSignature relevantSignature,
		std::shared_ptr <CompArray<Components>>... arrays) :
		m_AliveEntities(aliveEntites),
		m_Signatures(signatures),
		m_RelevantSignature(relevantSignature),
		m_Arrays{ arrays... }
	{}

	//TODO figure out a way for us to be able to get eihter individual elments or an entire tuple of all components we care about

	//TODO iterate over all alive entities, 
		//check the m_signatures if the current entity ID has m_relevant signature
		// then get refernces/ptrs to components from arrays and push them to the return array.

	std::vector<std::tuple<Components*...>> Get() {
		
		std::vector<std::tuple<Components*...>> table;
		for (EntityID id : m_AliveEntities)
		{
			if ((m_Signatures[id] & m_RelevantSignature) == m_RelevantSignature)
			{
				std::tuple<Components*...> tuple;

				std::apply([&tuple, id](std::shared_ptr<CompArray<Components>>... arrays)
					{
						//TODO for some reason the make tuple call here complains that RigidBodyComp is an unexpected type
						std::tie(std::get<Components*>(tuple)...) = std::make_tuple(std::addressof(arrays->GetComponent<Components>(id))...);
					}, 
					m_Arrays);

				table.push_back(tuple);
			}
		}
		return table;
	}

	/*template<typename T>
	std::vector<T*> Get() {

		std::vector<T*> collection;
		for (EntityID id : m_AliveEntities)
		{
			if ((m_Signatures[id] & m_RelevantSignature) == m_RelevantSignature)
			{
				std::shared_ptr <CompArray <T>> array = std::get<T>(m_Arrays);
				collection.push_back(array->GetComponent<T>(id));
			}
		}
		return collection;

	}*/

	DataTable<Components*...> GetAllComponentPairs()
	{
		DataTable<Components*...> table(Components*...);

		for (EntityID id : m_AliveEntities)
		{
			if ((m_Signatures[id] & m_RelevantSignature) == m_RelevantSignature)
			{
				std::tuple<Components*...> tuple;

				std::apply([&tuple, id](std::shared_ptr<CompArray<Components>>&... arrays)
					{
						// Use std::get to retrieve the array of components for each component type
						// and call GetComponent on it

						//GetAllComponentPairsImpl(tuple, id, std::make_index_sequence<sizeof...(Components)>{});
					
						//std::tie(std::get<Components*>(tuple)...) = std::make_tuple(std::addressof(arrays->GetComponent<Components>(id))...);
					},
					m_Arrays);

				table.push_back(tuple);
			}
		}

		return table;
	}

private:
	// Store references to the arrays of components.
	std::tuple<std::shared_ptr<CompArray<Components>>...> m_Arrays;

	CompSignature m_RelevantSignature;
	const std::array<CompSignature, MAX_ENTITIES>& m_Signatures;
	const std::unordered_set<EntityID>& m_AliveEntities;


	//void GetAllComponentPairsImpl(std::tuple<Components*...>& tuple, EntityID id, std::index_sequence<Indices...>)
	//{
	//	std::tie(std::get<Indices>(tuple)...) = std::make_tuple(std::addressof(arrays->GetComponent<Components>(id))...);

	//		//std::tie(std::get<Components*>(tuple)...) = std::make_tuple(std::addressof(arrays->GetComponent<Components>(id))...);
	//}
};
