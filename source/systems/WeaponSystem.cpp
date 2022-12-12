#include "WeaponSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"

void WeaponSystem::Update( float deltaTime)
{
	//TODO implement firerate if the player is holding fire button, so you don't have to release to fire again

	auto table = m_ComponentView->GetComponents();

	for (size_t i = 0; i < table.size(); i++)
	{
		auto input = std::get<1>(table[i]).InputSignature;
		auto& weapon = std::get<3>(table[i]);

		bool isInputingShoot = input[(int)Inputs::Shoot] == 1;

		if ( isInputingShoot && weapon.CanShoot)
		{
			auto tag = std::get<2>(table[i]).Tag;
			auto transform = std::get<0>(table[i]);
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

//TODO figure out way of spawning bullet without needing access to registry
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
