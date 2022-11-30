#pragma once
#include <string>

struct Vector3
{
	Vector3(float x,float y, float z) : X(x), Y(y), Z(z){}
	float X, Y, Z;

	(Vector3)operator = (Vector3 other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}
};

struct Vector2
{
	Vector2(float x, float y) : X(x), Y(y) {}
	float X, Y;

	(Vector2)operator = (Vector2 other)
	{
		X = other.X;
		Y = other.Y;
	}
};

struct TransformComp
{
	Vector2 Position;
	Vector2 Size;
	float Rotation;
};

struct NameComp 
{
	std::string Name;
};