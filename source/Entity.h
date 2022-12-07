#pragma once

#include <cassert>
#include <utility>
#include "ECSCore.h"
#include "Game.h"

#define ALIVE_CHECK() assert(IsAlive() && "Entity is dead! Cannot perform operation")

class Entity
{
public:
	Entity() = default;
	Entity(EntityID id, Game* game) : m_EntityID(id), m_Game(game), m_IsAlive(true) {}
	Entity(EntityID id) : m_EntityID(id) {}
	Entity(const Entity& other) = default;
	~Entity() = default;

	//template<typename T, typename... Args>
	//T& AddComponent(Args&&... args)
	//{
	//	ALIVE_CHECK();
	//	assert(!HasComponent<T>() && "Entity already has component!");
	//	//TODO investigaet furhter why we are not allowed to forward the args std::forward<Args>(args)...
	//	return m_Game->GetECSRegistry()->AddComponent<T>(m_EntityID,args);
	//}

	template<typename T>
	T& AddComponent()
	{
		ALIVE_CHECK();
		assert(!HasComponent<T>() && "Entity already has component!");
		return m_Game->GetECSRegistry()->AddComponent<T>(m_EntityID);
	}

	template<typename T>
	T& GetComponent() 
	{
		ALIVE_CHECK();
		assert(HasComponent<T>() && "Entity does not have component!");
		return  m_Game->GetECSRegistry()->GetComponent<T>(m_EntityID);
	}

	template<typename T>
	bool HasComponent()
	{
		ALIVE_CHECK();
		return m_Game->GetECSRegistry()->AnyOf<T>(m_EntityID);
	}

	template<typename T>
	void RemoveComponent()
	{
		ALIVE_CHECK();
		assert(HasComponent<T>() && "Entity does not have component!");
		m_Game->GetECSRegistry()->RemoveComponent<T>(m_EntityID);
	}

	bool operator==(const Entity& rhs) { return m_EntityID == rhs.m_EntityID; }

	operator bool() const { return m_EntityID != 0; }

	const EntityID GetID() { return m_EntityID; }
	const bool IsAlive() { return m_IsAlive && !m_IsQueuedForDestroy; }
	void SetAlive(bool b) { m_IsAlive = b; }
	void QueueDestroy() { m_IsQueuedForDestroy = true; }

	//TODO honestly everything in here should probably be its own component, instead of storing enites and signatures in entity admin
private:
	EntityID m_EntityID{};
	Game* m_Game = nullptr;
	bool m_IsAlive = false;
	bool m_IsQueuedForDestroy = false;
};
