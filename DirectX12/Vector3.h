#pragma once
#include <DirectXMath.h>

using XMFLOAT3 = DirectX::XMFLOAT3;

namespace KochaEngine
{
	struct Vector3 : public XMFLOAT3
	{
		Vector3();
		Vector3(float x, float y, float z);

		void Zero();

		float Length() const;
		Vector3& Normalize();
		float Dot(const Vector3& v) const;
		Vector3 Cross(const Vector3& v) const;

		Vector3 operator+() const;
		Vector3 operator-() const;

		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(const float s);
		Vector3& operator/=(const float s);
	};
}

//const KochaEngine::Vector3 operator+(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2);
//const KochaEngine::Vector3 operator-(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2);
//const KochaEngine::Vector3 operator*(const KochaEngine::Vector3& v, float s);
//const KochaEngine::Vector3 operator*(float s, const KochaEngine::Vector3& v);
//const KochaEngine::Vector3 operator/(const KochaEngine::Vector3& v, float s);
//
//const KochaEngine::Vector3 Lerp(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
//const KochaEngine::Vector3 EaseIn(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
//const KochaEngine::Vector3 EaseOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
//const KochaEngine::Vector3 EaseInOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);


inline KochaEngine::Vector3::Vector3()
	:Vector3(0, 0, 0)
{
}

inline KochaEngine::Vector3::Vector3(float x, float y, float z)
	: XMFLOAT3(x, y, z)
{
}

inline void KochaEngine::Vector3::Zero()
{
	x = y = z = 0;
}

inline float KochaEngine::Vector3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

inline KochaEngine::Vector3& KochaEngine::Vector3::Normalize()
{
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

inline float KochaEngine::Vector3::Dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline KochaEngine::Vector3 KochaEngine::Vector3::Cross(const Vector3& v) const
{
	return Vector3(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

inline KochaEngine::Vector3 KochaEngine::Vector3::operator+() const
{
	return *this;
}

inline KochaEngine::Vector3 KochaEngine::Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

inline KochaEngine::Vector3& KochaEngine::Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline KochaEngine::Vector3& KochaEngine::Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline KochaEngine::Vector3& KochaEngine::Vector3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline KochaEngine::Vector3& KochaEngine::Vector3::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline const KochaEngine::Vector3 operator+(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2)
{
	KochaEngine::Vector3 temp(v1);
	return temp += v2;
}

inline const KochaEngine::Vector3 operator-(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2)
{
	KochaEngine::Vector3 temp(v1);
	return temp -= v2;
}

inline const KochaEngine::Vector3 operator*(const KochaEngine::Vector3& v, float s)
{
	KochaEngine::Vector3 temp(v);
	return temp *= s;
}

inline const KochaEngine::Vector3 operator*(float s, const KochaEngine::Vector3& v)
{
	return v * s;
}

inline const KochaEngine::Vector3 operator/(const KochaEngine::Vector3& v, float s)
{
	KochaEngine::Vector3 temp(v);
	return temp /= s;
}

inline const KochaEngine::Vector3 Lerp(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

inline const KochaEngine::Vector3 EaseIn(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t)
{
	float y = t * t;
	return start * (1.0f - y) + end * y;
}

inline const KochaEngine::Vector3 EaseOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t)
{
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}

inline const KochaEngine::Vector3 EaseInOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t)
{
	float y = t * t * (3 - 2 * t);
	return start * (1.0f - y) + end * y;
}