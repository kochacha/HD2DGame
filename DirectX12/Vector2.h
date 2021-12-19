#pragma once
#include <DirectXMath.h>

using XMFLOAT2 = DirectX::XMFLOAT2;

namespace KochaEngine
{
	struct Vector2 : public XMFLOAT2
	{
		Vector2();
		Vector2(float x, float y);

		float length() const;
		Vector2& normalize();
		float dot(const Vector2& v) const;

		Vector2 operator+() const;
		Vector2 operator-() const;

		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(const float s);
		Vector2& operator/=(const float s);
	};
}

//const KochaEngine::Vector2 operator+(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2);
//const KochaEngine::Vector2 operator-(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2);
//const KochaEngine::Vector2 operator*(const KochaEngine::Vector2& v, float s);
//const KochaEngine::Vector2 operator*(float s, const KochaEngine::Vector2& v);
//const KochaEngine::Vector2 operator/(const KochaEngine::Vector2& v, float s);


inline KochaEngine::Vector2::Vector2() : Vector2(0, 0)
{
}

inline KochaEngine::Vector2::Vector2(float x, float y) : XMFLOAT2(x, y)
{
}

inline float KochaEngine::Vector2::length() const
{
	return sqrt(x * x + y * y);
}

inline KochaEngine::Vector2& KochaEngine::Vector2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

inline float KochaEngine::Vector2::dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

inline KochaEngine::Vector2 KochaEngine::Vector2::operator+() const
{
	return *this;
}

inline KochaEngine::Vector2 KochaEngine::Vector2::operator-() const
{
	return Vector2(-x, -y);
}

inline KochaEngine::Vector2& KochaEngine::Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

inline KochaEngine::Vector2& KochaEngine::Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

inline KochaEngine::Vector2& KochaEngine::Vector2::operator*=(const float s)
{
	x *= s;
	y *= s;
	return *this;
}

inline KochaEngine::Vector2& KochaEngine::Vector2::operator/=(const float s)
{
	x /= s;
	y /= s;
	return *this;
}

inline const KochaEngine::Vector2 operator+(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2)
{
	KochaEngine::Vector2 temp(v1);
	return temp += v2;
}

inline const KochaEngine::Vector2 operator-(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2)
{
	KochaEngine::Vector2 temp(v1);
	return temp -= v2;
}

inline const KochaEngine::Vector2 operator*(const KochaEngine::Vector2& v, float s)
{
	KochaEngine::Vector2 temp(v);
	return temp *= s;
}

inline const KochaEngine::Vector2 operator*(float s, const KochaEngine::Vector2& v)
{
	return v * s;
}

inline const KochaEngine::Vector2 operator/(const KochaEngine::Vector2& v, float s)
{
	KochaEngine::Vector2 temp(v);
	return temp /= s;
}
