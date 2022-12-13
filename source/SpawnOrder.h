#pragma once
#include "Components.h"

struct SpawnOrder
{
public:
	SpawnOrder(Vector2 spawnPos, ObjectTag tag, std::bitset<5> sig) : SpawnPosition(spawnPos), Tag(tag), InputSignature(sig) {}

	Vector2 SpawnPosition = Vector2(0.f, 0.f);
	ObjectTag Tag = ObjectTag::None;
	std::bitset<5> InputSignature{};
};