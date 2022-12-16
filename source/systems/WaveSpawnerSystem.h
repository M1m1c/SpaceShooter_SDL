#pragma once
#include "ISystem.h"
#include "../ECS/ECSCore.h"
#include "../ECS/Components.h"
#include "../containers/FixedQueue.h"
#include <array>
#include <random>

struct SpawnOrder;

struct SpawnParams 
{
public:
	SpawnParams(Vector2 startPos, Vector2 offset, std::bitset<5> sig, size_t count=10, float startOffset=100.f) : StartPosition(startPos),Offset(offset),StartOffset(startOffset), InputSignature(sig), Count(count) {}
	
	Vector2 StartPosition = Vector2(0.f,0.f);
	Vector2 Offset = Vector2(120.f, 0.f);
	float StartOffset = 100.f;
	std::bitset<5> InputSignature;
	size_t Count;
};

class WaveSpawnerSystem : public ISystem
{
public:
	WaveSpawnerSystem(FixedQueue<SpawnOrder, MAX_ENTITIES>& orderQueue) : m_OrderQueue(orderQueue) { }
	~WaveSpawnerSystem() = default;

	virtual void Update(float deltaTime) override;
private:

	FixedQueue<SpawnOrder, MAX_ENTITIES>& m_OrderQueue;

	std::random_device m_RD;

	float m_WaveSpawnTime = 2.f;
	float m_MinWaveSpawnTime = 1.f;
	float m_MaxWaveSpawnTime = 2.f;

	std::array<SpawnParams, 7> m_SpawnPatterns{
		SpawnParams(Vector2(120.f,0.f),Vector2(120.f,0.f), std::bitset<5>(1<<(int)Inputs::Down),10),
		SpawnParams(Vector2(400.f,0.f),Vector2(50.f,0.f), std::bitset<5>(1<<(int)Inputs::Down),10,200.f),
		SpawnParams(Vector2(15.f,0.f),Vector2(50.f,0.f), std::bitset<5>(1<<(int)Inputs::Down),50,0.f),
		SpawnParams(Vector2(1280.f,-300.f),Vector2(120.f,-100.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Left)),20),
		SpawnParams(Vector2(1280.f,-300.f),Vector2(50.f,-50.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Left)),10),
		SpawnParams(Vector2(0.f,-300.f),Vector2(-120.f,-100.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Right)),20),
		SpawnParams(Vector2(0.f,-300.f),Vector2(-50.f,-50.f), std::bitset<5>((1<<(int)Inputs::Down) + (1<<(int)Inputs::Right)),10)
	};
};

