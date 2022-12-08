#pragma once
#include <string>
#include <bitset>
#include <cmath>
#include <glm/glm.hpp>

using Vector4 = glm::vec4;
using Vector2 = glm::vec2;

enum class Inputs
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3,
	Shoot = 4,
};

enum class ObjectTag
{
	None,
	Player = 1 << 0,
	Enemy = 1 << 1,
	Bullet = 1 << 2
};

struct TransformComp
{
	Vector2 Position;
	Vector2 Size;

	TransformComp() = default;
	TransformComp(Vector2 position,Vector2 size): Position(position), Size(size){}
	~TransformComp() = default;
};

struct NameComp
{
	std::string Name;
};

struct RigidBodyComp
{
	Vector2 velocity;
	float acceleration = 250.f;
};

struct InputComp
{
	std::bitset<5> InputSignature = 0;
};

struct TagComp 
{
	ObjectTag Tag;

	TagComp() = default;
	TagComp(ObjectTag tag) : Tag(tag){}
	~TagComp() = default;
};

struct WeaponComp
{
	bool CanShoot = true;
};

struct HealthComp
{
	uint16_t Health;
	bool IsQueuedForDestroy = false;

	HealthComp() = default;
	HealthComp(uint16_t health) : Health(health){}
	~HealthComp() = default;
};
