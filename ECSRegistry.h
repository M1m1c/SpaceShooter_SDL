#pragma once

#include <string>
#include <utility>
#include "ECSCore.h"
#include "EntityAdmin.h"
#include "ComponentAdmin.h"
#include "Entity.h"
#include "Scene.h"

class ECSRegistry
{
public:
	void Init()
	{
		m_ComponentAdmin = std::make_unique<ComponentAdmin>();
		m_EntityAdmin = std::make_unique<EntityAdmin>();
	}


	Entity CreateEntity(const std::string& name, Scene* activeScene)
	{
		auto temp = Entity(m_EntityAdmin->CreateEntity(), activeScene);
		//TODO make sure adding NameComp works, maybe not here since we can't include Components.h
		//temp.AddComponent<NameComp>(name);
		return temp;
	}

	Entity CreateEntity(const std::string& name)
	{
		return Entity(m_EntityAdmin->CreateEntity());
	}

	void DestroyEntity(EntityID entityID)
	{
		m_EntityAdmin->DestroyEntity(entityID);

		m_ComponentAdmin->EntityDestroyed(entityID);
	}

	template<typename T>
	void RegisterComponent()
	{
		m_ComponentAdmin->RegisterComponent<T>();
	}

	template<typename T, typename... Args>
	void AddComponent(EntityID entityID, Args&&... args)
	{
		m_ComponentAdmin->AddComponent<T>(entityID, std::forward<Args>(args)...);

		auto signature = m_EntityAdmin->GetSignature(entityID);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), true);
		m_EntityAdmin->SetSignature(entityID, signature);
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