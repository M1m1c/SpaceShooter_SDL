#pragma once

#include <string>
#include <utility>
#include <memory>
#include "ECSCore.h"
#include "EntityAdmin.h"
#include "ComponentAdmin.h"
#include "SystemsViewAdmin.h"
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
		m_SystemsViewAdmin = std::make_unique<SystemsViewAdmin>(m_ComponentAdmin);
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

		m_ComponentAdmin->OnEntityDestroyed(entityID);

		m_SystemsViewAdmin->OnEntityDestroyed(entityID);
	}

	template<typename... Components>
	std::shared_ptr<ComponentView<Components...>> CreateComponentView()
	{
		auto signature = ComposeSignature<Components...>();
		return std::make_shared<ComponentView<Components...>>(signature);
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
		m_ComponentAdmin->AddComponent<T>(entityID, args...);

		auto signature = m_EntityAdmin->GetSignature(entityID);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), true);
		m_EntityAdmin->SetSignature(entityID, signature);

		m_SystemsViewAdmin->OnComponentAdded(entityID, signature);

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

	template< typename... Types>
	std::vector<EntityID> GetEntityIDsMatchingSignature()
	{
		CompSignature signature = ComposeSignature<Types...>();
		size_t entityCount = 0;

		return m_EntityAdmin->GetEntitiesWithMatchingSignature(signature);
	}


	const std::array<CompSignature, MAX_ENTITIES>& GetSignatures()
	{
		return m_EntityAdmin->GetSignatures();
	}

	template<typename T>
	bool AnyOf(EntityID entityID)
	{
		CompSignature signature = m_EntityAdmin->GetSignature(entityID);
		CompType type = m_ComponentAdmin->GetComponentType<T>();
		return signature[type];
	}

	void SetThrottleView(std::shared_ptr <ComponentView<RigidBodyComp, InputComp>> view) { m_SystemsViewAdmin->m_ThrottleView = view; }
	void SetRenderView(std::shared_ptr <ComponentView<TransformComp, TagComp>> view) { m_SystemsViewAdmin->m_RenderView = view; }
	void SetWeaponView(std::shared_ptr <ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> view) { m_SystemsViewAdmin->m_WeaponView = view; }
	void SetMoveTranslateView(std::shared_ptr <ComponentView<TransformComp, RigidBodyComp, TagComp, HealthComp>> view) { m_SystemsViewAdmin->m_MoveTranslateView = view; }

private:
	std::unique_ptr<ComponentAdmin> m_ComponentAdmin;
	std::unique_ptr<EntityAdmin> m_EntityAdmin;
	std::unique_ptr<SystemsViewAdmin> m_SystemsViewAdmin;
};