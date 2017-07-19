#pragma once
#include <Engine/Util/json/json.h>
class Vector2
{
public:
	float x, y;
	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(Json::Value obj)
	{
		x = obj["x"].asFloat();
		y = obj["y"].asFloat();
	}

	Vector2(float a, float b)
	{
		x = a;
		y = b;
	}

	float Magnitude()
	{
		return sqrt(x*x + y*y);
	}

	friend Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}
	friend Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}
	friend Vector2 operator*(const Vector2& a, float b)
	{
		return Vector2(a.x * b, a.y * b);
	}
	friend Vector2 operator/(const Vector2& a, float b)
	{
		return Vector2(a.x / b, a.y / b);
	}
};
