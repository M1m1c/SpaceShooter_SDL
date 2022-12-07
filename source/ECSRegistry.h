#pragma once

#include <string>
#include <utility>
#include "ECSCore.h"
#include "EntityAdmin.h"
#include "ComponentAdmin.h"
#include "Entity.h"
#include "Game.h"
#include "DataTable.h"



class ECSRegistry
{
public:
	void Init()
	{
		m_ComponentAdmin = std::make_unique<ComponentAdmin>();
		m_EntityAdmin = std::make_unique<EntityAdmin>();
	}


	Entity& CreateEntity(const std::string& name, Game* activeGame)
	{
		//auto& temp = m_EntityAdmin->CreateEntity(activeGame);// Entity(, activeGame);
		//TODO make sure adding NameComp works, maybe not here since we can't include Components.h
		//temp.AddComponent<NameComp>(name);
		return m_EntityAdmin->CreateEntity(activeGame);
	}

	Entity& GetEntity(EntityID entity)
	{
		return m_EntityAdmin->GetEntity(entity);
	}

	/*Entity CreateEntity(const std::string& name)
	{
		return Entity(m_EntityAdmin->CreateEntity());
	}*/

	void DestroyEntity(EntityID entityID)
	{
		m_EntityAdmin->DestroyEntity(entityID);

		m_ComponentAdmin->EntityDestroyed(entityID);
	}

	uint32_t GetLivingEntities()
	{
		return m_EntityAdmin->GetLivingEntities();
	}

	template<typename T>
	void RegisterComponent()
	{
		m_ComponentAdmin->RegisterComponent<T>();
	}

	template<typename T>
	T& AddComponent(EntityID entityID)
	{
		m_ComponentAdmin->AddComponent<T>(entityID);

		auto signature = m_EntityAdmin->GetSignature(entityID);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), true);
		m_EntityAdmin->SetSignature(entityID, signature);
		return m_ComponentAdmin->GetComponent<T>(entityID);
	}

	template<typename T>
	void RemoveComponent(EntityID entityID)
	{
		m_ComponentAdmin->RemoveComponent<T>(entityID);

		auto signature = m_EntityAdmin->GetSignature(entityID);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), false);
		m_EntityAdmin->SetSignature(entityID, signature);
	}

	template<typename T>
	T& GetComponent(EntityID entityID)
	{
		return m_ComponentAdmin->GetComponent<T>(entityID);
	}

	template<typename T>
	CompType GetComponentType()
	{
		return m_ComponentAdmin->GetComponentType<T>();
	}

	template<typename T, typename... Types>
	CompSignature& SignSignature(CompSignature& signature)
	{
		signature.set(GetComponentType<T>());
		(SignSignature<Types>(signature), ...);
		return signature;
	}

	template<typename... Types>
	CompSignature& ComposeSignature()
	{
		CompSignature retSignature;
		return (SignSignature<Types>(retSignature), ...);
	}

	uint32_t GetMatchingSignatureCount(const CompSignature& signature)
	{
		return m_EntityAdmin->GetMatchingSignatureCount(signature);
	}

	//TODO for some reason this does not return refernces to teh components, try to figure out why
	//Composes a DataTable& where each entry corresponds to the components of one entity,
	//based on the sent in componenttypes.
	//Returns the number of entities that have these components.
	template<size_t size, typename... Types>
	size_t GetAllComponentPairs(DataTable<size, Types...>& outTable)
	{
		CompSignature signature = (ComposeSignature<Types>(), ...);
		size_t entityCount = 0;

		std::vector<EntityID> entityIDs = m_EntityAdmin->GetEntitiesWithMatchingSignature(signature);
		entityCount = entityIDs.size();

		for (size_t i = 0; i < entityCount; i++)
		{
			outTable.push_back(std::tie(GetComponent<Types>(entityIDs[i])...));
		}

		return entityCount;
	}


	template< typename... Types>
	std::vector<EntityID> GetEntityIDsMatchingSignature()
	{
		CompSignature signature = (ComposeSignature<Types>(), ...);
		size_t entityCount = 0;

		return m_EntityAdmin->GetEntitiesWithMatchingSignature(signature);
	}


	template<typename T>
	bool AnyOf(EntityID entityID)
	{
		CompSignature signature = m_EntityAdmin->GetSignature(entityID);
		CompType type = m_ComponentAdmin->GetComponentType<T>();
		return signature[type];
	}

private:
	std::unique_ptr<ComponentAdmin> m_ComponentAdmin;
	std::unique_ptr<EntityAdmin> m_EntityAdmin;
};