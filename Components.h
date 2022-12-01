#pragma once
#include <string>
#include <bitset>
#include <cmath>

struct Vector3
{
	Vector3() = default;
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	float X, Y, Z;

	/*(Vector3)operator = (Vector3 other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}*/
};

struct Vector2
{
	Vector2() = default;
	Vector2(float x, float y) : X(x), Y(y) {}
	float X, Y;

	Vector2& operator = (const Vector2& other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}

	Vector2& operator += (const Vector2& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}

	Vector2& operator* (const float& other) 
	{	
		X *= other;
		Y *= other;
		return *this;
	}

	//void Normalize() 
	//{
	//	float length = std::sqrt((X * X) + (Y * Y));
	//	X = X / length;
	//	Y = Y / length;
	//}
};

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
	/*TransformComp(const TransformComp&) = default;
	TransformComp(Vector2 position = { 0.f,0.f }, Vector2 size = { 0.f,0.f }, float rotation = 0.f) :
		Position(position), Size(size), Rotation(rotation) {}*/
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
