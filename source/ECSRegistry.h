#pragma once

#include <string>
#include <utility>
#include <memory>
#include "ECSCore.h"
#include "EntityAdmin.h"
#include "ComponentAdmin.h"
#include "Components.h"
#include "Game.h"
#include "ComponentView.h"



class ECSRegistry
{
public:
	void Init()
	{
		m_ComponentAdmin = std::make_unique<ComponentAdmin>();
		m_EntityAdmin = std::make_unique<EntityAdmin>();
	}

	template<typename... Types>
	EntityID& CreateEntity(
		Vector4 transformValues = Vector4(0.f, 0.f, 1.f, 1.f),
		ObjectTag tag = ObjectTag::None,
		uint16_t health = 1)
	{
		auto id = m_EntityAdmin->CreateEntity();

		AddComponent<TransformComp>(
			id,
			Vector2(transformValues.x, transformValues.y),
			Vector2(transformValues.z, transformValues.w));

		AddComponent<TagComp>(id, tag);
		AddComponent<HealthComp>(id, health);

		(AddComponent<Types>(id), ...);

		return id;
	}

	void DestroyEntity(EntityID entityID)
	{
		m_EntityAdmin->DestroyEntity(entityID);

		m_ComponentAdmin->EntityDestroyed(entityID);
	}

	template<typename... Components>
	std::shared_ptr<ComponentView<Components...>> CreateComponentView() 
	{
		return std::make_shared<ComponentView<Components...>>(
			GetActiveEntities(),
			m_EntityAdmin->Getsignatures(),
			ComposeSignature<Components...>(),
			(m_ComponentAdmin->GetComponentArray<Components>())...);
	}


	uint32_t GetLivingEntitiesCount()
	{
		return m_EntityAdmin->GetLivingEntitiesCount();
	}

	const std::unordered_set<EntityID>& GetActiveEntities()
	{
		return   m_EntityAdmin->GetActiveEntities();
	}

	template<typename T>
	void RegisterComponent()
	{
		m_ComponentAdmin->RegisterComponent<T>();
	}

	template<typename T, typename... Args>
	T& AddComponent(EntityID entityID, Args&&... args)
	{
		m_ComponentAdmin->AddComponent<T>(entityID,args...);

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
	template<typename... Types>
	size_t GetAllComponentPairs(DataTable<Types...>& outTable)
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

	/*const std::array<CompSignature, MAX_ENTITIES>& Getsignatures()
	{
		return m_EntityAdmin->Getsignatures();
	}

	template<typename T>
	std::shared_ptr<CompArray<T>> GetComponentArray()
	{
		return m_ComponentAdmin->GetComponentArray<T>();
	}*/

private:
	std::unique_ptr<ComponentAdmin> m_ComponentAdmin;
	std::unique_ptr<EntityAdmin> m_EntityAdmin;
};