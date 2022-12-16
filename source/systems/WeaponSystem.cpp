#include "WeaponSystem.h"
#include "../ECS/Components.h"
#include "SpawnOrder.h"

void WeaponSystem::Update( float deltaTime)
{
	auto& table = m_SystemView->GetComponents();

	for (size_t i = 0; i < table.size(); i++)
	{
		auto input = std::get<1>(table[i]).InputSignature;
		auto& weapon = std::get<3>(table[i]);

		bool isInputingShoot = input[(int)Inputs::Shoot] == 1;

		if ( isInputingShoot)
		{
			weapon.CoolDown -= deltaTime;

			if ((weapon.CoolDown <= 0.f && !weapon.CanShoot))
			{
				weapon.CanShoot = true;	
			}
			else if(!weapon.CanShoot)
			{
				continue;
			}

			auto tag = std::get<2>(table[i]).Tag;
			auto transform = std::get<0>(table[i]);
			weapon.CanShoot = false;
			weapon.CoolDown = weapon.GetMaxCoolDown();

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
			weapon.CoolDown = weapon.GetMaxCoolDown();
		}
	}
}

void WeaponSystem::SpawnBullet(glm::vec2 position,int direction)
{
	m_OrderQueue.push(SpawnOrder(position, ObjectTag::Bullet, std::bitset<5>(1 << direction)));
}
