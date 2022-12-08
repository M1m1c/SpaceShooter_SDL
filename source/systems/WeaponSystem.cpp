#include "WeaponSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"
#include "../Game.h"
#include <glm/gtc/epsilon.hpp>

void WeaponSystem::Update( float deltaTime)
{
	//TODO implement firerate if the player is holding fire button, so you don't have to release to fire again

	auto entityIDs = m_Registry->GetEntityIDsMatchingSignature<TransformComp,InputComp, TagComp,WeaponComp>();

	for (size_t i = 0; i < entityIDs.size(); i++)
	{
		auto input = m_Registry->GetComponent<InputComp>(entityIDs[i]).InputSignature;
		auto& weapon = m_Registry->GetComponent<WeaponComp>(entityIDs[i]);

		bool isInputingShoot = input[(int)Inputs::Shoot] == 1;

		if ( isInputingShoot && weapon.CanShoot)
		{
			auto tag = m_Registry->GetComponent<TagComp>(entityIDs[i]).Tag;
			auto transform = m_Registry->GetComponent<TransformComp>(entityIDs[i]);
			weapon.CanShoot = false;

			switch (tag)
			{
			case ObjectTag::None:
				break;
			case ObjectTag::Player:
				
				SpawnBullet(transform.Position - glm::vec2(0.f, transform.Size.y), (int)Inputs::Up);
				break;
			case ObjectTag::Enemy:
				SpawnBullet(transform.Position + glm::vec2(0.f, transform.Size.y), (int)Inputs::Down);
				break;
			case ObjectTag::Bullet:
				break;
			default:
				break;
			}
		}
		else if(!isInputingShoot)
		{
			weapon.CanShoot = true;
		}
	}
}

void WeaponSystem::SpawnBullet(glm::vec2 position,int direction)
{

	auto bullet = m_Registry->CreateEntity<RigidBodyComp, InputComp>(
		Vector4(position.x, position.y, 5.f, 5.f),
		ObjectTag::Bullet);

	auto& rigidBody = m_Registry->GetComponent<RigidBodyComp>(bullet);
	auto& input = m_Registry->GetComponent<InputComp>(bullet);

	rigidBody.acceleration = 500.f;

	input.InputSignature[direction] = 1;
}
