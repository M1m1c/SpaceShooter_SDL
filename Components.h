#pragma once

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

struct Position
{
	Vector3 Position;
};