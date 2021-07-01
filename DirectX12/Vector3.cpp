#include "Vector3.h"
#include <cmath>

KochaEngine::Vector3::Vector3()
	:Vector3(0, 0, 0)
{
}

KochaEngine::Vector3::Vector3(float x, float y, float z)
	: XMFLOAT3(x, y, z)
{
}

void KochaEngine::Vector3::Zero()
{
	x = y = z = 0;
}

float KochaEngine::Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

KochaEngine::Vector3& KochaEngine::Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float KochaEngine::Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

KochaEngine::Vector3 KochaEngine::Vector3::cross(const Vector3 & v) const
{
	return Vector3(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

KochaEngine::Vector3 KochaEngine::Vector3::operator+() const
{
	return *this;
}

KochaEngine::Vector3 KochaEngine::Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

KochaEngine::Vector3& KochaEngine::Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

KochaEngine::Vector3& KochaEngine::Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

KochaEngine::Vector3& KochaEngine::Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

KochaEngine::Vector3& KochaEngine::Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const KochaEngine::Vector3 operator+(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2)
{
	KochaEngine::Vector3 temp(v1);
	return temp += v2;
}

const KochaEngine::Vector3 operator-(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2)
{
	KochaEngine::Vector3 temp(v1);
	return temp -= v2;
}

const KochaEngine::Vector3 operator*(const KochaEngine::Vector3& v, float s)
{
	KochaEngine::Vector3 temp(v);
	return temp *= s;
}

const KochaEngine::Vector3 operator*(float s, const KochaEngine::Vector3& v)
{
	return v * s;
}

const KochaEngine::Vector3 operator/(const KochaEngine::Vector3& v, float s)
{
	KochaEngine::Vector3 temp(v);
	return temp /= s;
}

const KochaEngine::Vector3 lerp(const KochaEngine::Vector3 & start, const KochaEngine::Vector3 & end, const float t)
{
	return start * (1.0f - t) + end * t;
}

const KochaEngine::Vector3 easeIn(const KochaEngine::Vector3 & start, const KochaEngine::Vector3 & end, const float t)
{
	float y = t * t;
	return start * (1.0f - y) + end * y;
}

const KochaEngine::Vector3 easeOut(const KochaEngine::Vector3 & start, const KochaEngine::Vector3 & end, const float t)
{
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}

const KochaEngine::Vector3 easeInOut(const KochaEngine::Vector3 & start, const KochaEngine::Vector3 & end, const float t)
{
	float y = t * t * (3 - 2 * t);
	return start * (1.0f - y) + end * y;
}

