#pragma once
#include "ISystem.h"
#include "../Components.h"
#include <array>
#include <random>
class ECSRegistry;

struct SpawnParams 
{
public:
	SpawnParams(Vector2 startPos, Vector2 offset, std::bitset<5> sig) : StartPosition(startPos),Offset(offset), InputSignature(sig) {}
	
	Vector2 StartPosition = Vector2(0.f,0.f);
	Vector2 Offset = Vector2(120.f, 0.f);
	std::bitset<5> InputSignature;
};

class EnemySpawnerSystem : public ISystem
{
public:
	EnemySpawnerSystem(const std::shared_ptr<ECSRegistry>& registry) : m_Registry(registry){ }
	~EnemySpawnerSystem() = default;

	virtual void Update(float deltaTime) override;
private:

	const std::shared_ptr<ECSRegistry>& m_Registry;

	std::random_device m_RD;

	float m_WaveSpawnTime = 2.f;
	float m_MinWaveSpawnTime = 1.f;
	float m_MaxWaveSpawnTime = 4.f;

	std::array<SpawnParams, 4> m_SpawnPatterns{
		SpawnParams(Vector2(120.f,0.f),Vector2(120.f,0.f), std::bitset<5>(1<<(int)Inputs::Down)),
		SpawnParams(Vector2(400.f,0.f),Vector2(50.f,0.f), std::bitset<5>(1<<(int)Inputs::Down)),
		SpawnParams(Vector2(1280.f,-300.f),Vector2(120.f,-100.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Left))),
		SpawnParams(Vector2(0.f,-300.f),Vector2(-120.f,-100.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Right)))
	};
};

