#include "WeaponSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Entity.h"
#include "../Components.h"
#include "../Game.h"
#include <glm/gtc/epsilon.hpp>

void WeaponSystem::Update(const std::shared_ptr<ECSRegistry>& registry, Game* game, float deltaTime)
{
	//TODO get if entity has input fire held down, create a new entity that has transform, input, rigidbody, tagcomp.
	//it should be of tagtype bullet and move upwards if player fired, downwards if enemy fired.

	auto entityIDs = registry->GetEntityIDsMatchingSignature<TransformComp,InputComp, TagComp,WeaponComp>();

	for (size_t i = 0; i < entityIDs.size(); i++)
	{
		auto input = registry->GetComponent<InputComp>(entityIDs[i]).InputSignature;
		auto& weapon = registry->GetComponent<WeaponComp>(entityIDs[i]);

		bool isInputingShoot = input[(int)Inputs::Shoot] == 1;

		if ( isInputingShoot && weapon.CanShoot)
		{
			auto tag = registry->GetComponent<TagComp>(entityIDs[i]).Tag;
			auto transform = registry->GetComponent<TransformComp>(entityIDs[i]);
			weapon.CanShoot = false;

			switch (tag)
			{
			case ObjectTag::None:
				break;
			case ObjectTag::Player:
				SpawnBullet(game,transform.Position,(int)Inputs::Up);
				break;
			case ObjectTag::Enemy:
				SpawnBullet(game, transform.Position, (int)Inputs::Down);
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

void WeaponSystem::SpawnBullet(Game* game,glm::vec2 position,int direction)
{
	auto& bullet = game->CreateEntity("bullet");

	auto& transform = bullet.AddComponent<TransformComp>();
	auto& rigidBody = bullet.AddComponent<RigidBodyComp>();
	auto& input = bullet.AddComponent<InputComp>();
	auto& tag = bullet.AddComponent<TagComp>();


	transform.Position = position;
	transform.Size = glm::vec2(5.f, 5.f);

	tag.Tag = ObjectTag::Bullet;

	input.InputSignature[direction] = 1;
}
