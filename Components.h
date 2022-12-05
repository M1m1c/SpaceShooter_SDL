#pragma once
#include <string>
#include <bitset>
#include <cmath>
#include <glm/glm.hpp>

using Vector2 = glm::vec2;

enum Inputs
{
	Up =	0,
	Down =	1,
	Left =	2,
	Right = 3,
	Shoot = 4,
};

struct TransformComp
{
	Vector2 Position;
	Vector2 Size;
	float Rotation;

	TransformComp() = default;
	~TransformComp() = default;
};

struct NameComp
{
	std::string Name;
};

struct RigidBodyComp
{
	Vector2 velocity;
	Vector2 acceleration;
};

struct InputComp
{
	std::bitset<5> InputSignature = 0;
};
