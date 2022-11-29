#pragma once

#include <cassert>
#include <memory>
#include "ECSCore.h"
#include "CompArray.h"

class ComponentAdmin
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

		m_ComponentTypes.insert({ typeName, m_NextComponentType });

		m_ComponentArrays.insert({ typeName, std::make_shared<CompArray<T>>() });

		++m_NextComponentType;
	}

	template<typename T>
	CompType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

		return m_ComponentTypes[typeName];
	}

	template<typename T, typename... Args>
	void AddComponent(EntityID entity, Args&&... args)
	{
		GetComponentArray<T>()->AddComponent(entity, T(args));
	}

	template<typename T>
	void RemoveComponent(EntityID entity)
	{
		GetComponentArray<T>()->RemoveComponent(entity);
	}

	template<typename T>
	T& GetComponent(EntityID entity)
	{
		return GetComponentArray<T>()->GetComponent(entity);
	}

	void EntityDestroyed(EntityID entity)
	{
		for (auto const& pair : m_ComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

private:
	std::unordered_map<const char*, CompType> m_ComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<ICompArray>> m_ComponentArrays{};
	CompType m_NextComponentType{};
	template<typename T>
	std::shared_ptr<CompArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<CompArray<T>>(m_ComponentArrays[typeName]);
	}
};