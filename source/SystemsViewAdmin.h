#pragma once

#include "ECSCore.h"
#include "ECSRegistry.h"
#include "ComponentView.h"
#include "Components.h"
#include <unordered_map>
#include <memory>

class SystemsViewAdmin
{
public:
	SystemsViewAdmin(const std::unique_ptr<ComponentAdmin>& compAdmin) : m_CompAdmin(compAdmin) {}
	~SystemsViewAdmin() = default;

	void OnComponentAdded(const EntityID& id, const CompSignature& signature)
	{
		const auto& throttleSignature = m_ThrottleView->GetRelevantSignature();
		if ((signature & throttleSignature) == throttleSignature)
		{
			if (!m_ThrottleView->ContainsEntity(id))
			{
				m_ThrottleView->Add(id, MakeTableEntry<RigidBodyComp, InputComp>(id));
			}
		}


		const auto& renderSignature = m_RenderView->GetRelevantSignature();
		if ((signature & renderSignature) == renderSignature)
		{
			if (!m_RenderView->ContainsEntity(id))
			{
				m_RenderView->Add(id, MakeTableEntry<TransformComp, TagComp>(id));
			}
		}


		const auto& weaponSignature = m_WeaponView->GetRelevantSignature();
		if ((signature & weaponSignature) == weaponSignature)
		{
			if (!m_WeaponView->ContainsEntity(id))
			{
				m_WeaponView->Add(id, MakeTableEntry<TransformComp, InputComp, TagComp, WeaponComp>(id));
			}
		}


		const auto& moveTranslateSignature = m_MoveTranslateView->GetRelevantSignature();
		if ((signature & moveTranslateSignature) == moveTranslateSignature)
		{
			if (!m_MoveTranslateView->ContainsEntity(id))
			{
				m_MoveTranslateView->Add(id, MakeTableEntry<TransformComp, RigidBodyComp, TagComp, HealthComp>(id));
			}
		}

	}

	

	void OnEntityDestroyed(const EntityID& id)
	{
		if (!m_ThrottleView->ContainsEntity(id))
		{
			m_ThrottleView->Remove(id);
		}

		if (!m_RenderView->ContainsEntity(id))
		{
			m_RenderView->Remove(id);
		}

		if (!m_WeaponView->ContainsEntity(id))
		{
			m_WeaponView->Remove(id);
		}

		if (!m_MoveTranslateView->ContainsEntity(id))
		{
			m_MoveTranslateView->Remove(id);
		}
	}

	template<typename... Components>
	std::tuple<Components&...> MakeTableEntry(const EntityID& id)
	{
		return std::tie(*&(m_CompAdmin->GetComponent<Components>(id))...);
	}

public:

	std::shared_ptr <ComponentView<RigidBodyComp, InputComp>> m_ThrottleView;
	std::shared_ptr <ComponentView<TransformComp, TagComp>> m_RenderView;
	std::shared_ptr <ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> m_WeaponView;
	std::shared_ptr <ComponentView<TransformComp, RigidBodyComp, TagComp, HealthComp>> m_MoveTranslateView;

private:
	std::unordered_map<size_t, std::shared_ptr<ICompArray>> m_ComponentArrays{};
	const std::unique_ptr<ComponentAdmin>& m_CompAdmin;
};
