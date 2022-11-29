#pragma once

#include <cassert>
#include <utility>
#include "ECSCore.h"
#include "Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(EntityID id, Scene* scene) : m_EntityID(id), m_Scene(scene) {}
	Entity(EntityID id) : m_EntityID(id){}
	Entity(const Entity& other) = default;
	~Entity() = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args) 
	{
		assert(!HasComponent<T>() && "Entity already has component!");
		//TODO investigaet furhter why we are not allowed to forward the args std::forward<Args>(args)...
		return m_Scene->GetECSRegistry()->AddComponent<T>(m_EntityID, args);
	}

	template<typename T>
	T& GetComponent() {
		assert(HasComponent<T>() && "Entity does not have component!");
		return  m_Scene->GetECSRegistry()->GetComponent<T>(m_EntityID);
	}

	template<typename T>
	bool HasComponent() 
	{	
		return m_Scene->GetECSRegistry()->AnyOf<T>(m_EntityID);
	}

	template<typename T>
	void RemoveComponent() 
	{
		assert(HasComponent<T>() && "Entity does not have component!");
		m_Scene->GetECSRegistry()->RemoveComponent<T>(m_EntityID);
	}

	bool operator==(const Entity& rhs) { return m_EntityID == rhs.m_EntityID; }

	operator bool() const { return m_EntityID != 0; }

	const EntityID GetID() { return m_EntityID; }

private:
	EntityID m_EntityID{ 0 }; //0 is to be treated as an invalid ID
	Scene* m_Scene = nullptr;
};
