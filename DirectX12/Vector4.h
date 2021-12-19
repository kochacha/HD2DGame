#pragma once
#include <DirectXMath.h>

using XMFLOAT4 = DirectX::XMFLOAT4;

namespace KochaEngine
{
	struct Vector4 : public XMFLOAT4
	{
		Vector4();
		Vector4(float x, float y, float z, float w);

		void Zero();

		Vector4 operator+() const;
		Vector4 operator-() const;

		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(const float s);
		Vector4& operator/=(const float s);
	};
}

//const KochaEngine::Vector4 operator+(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2);
//const KochaEngine::Vector4 operator-(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2);
//const KochaEngine::Vector4 operator*(const KochaEngine::Vector4& v, float s);
//const KochaEngine::Vector4 operator*(float s, const KochaEngine::Vector4& v);
//const KochaEngine::Vector4 operator/(const KochaEngine::Vector4& v, float s);


inline KochaEngine::Vector4::Vector4()
	:Vector4(0, 0, 0, 0)
{
}

inline KochaEngine::Vector4::Vector4(float x, float y, float z, float w)
	: XMFLOAT4(x, y, z, w)
{
}

inline void KochaEngine::Vector4::Zero()
{
	x = y = z = w = 0;
}

inline KochaEngine::Vector4 KochaEngine::Vector4::operator+() const
{
	return *this;
}

inline KochaEngine::Vector4 KochaEngine::Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

inline KochaEngine::Vector4& KochaEngine::Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline KochaEngine::Vector4& KochaEngine::Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline KochaEngine::Vector4& KochaEngine::Vector4::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline KochaEngine::Vector4& KochaEngine::Vector4::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

inline const KochaEngine::Vector4 operator+(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2)
{
	KochaEngine::Vector4 temp(v1);
	return temp += v2;
}

inline const KochaEngine::Vector4 operator-(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2)
{
	KochaEngine::Vector4 temp(v1);
	return temp -= v2;
}

inline const KochaEngine::Vector4 operator*(const KochaEngine::Vector4& v, float s)
{
	KochaEngine::Vector4 temp(v);
	return temp *= s;
}

inline const KochaEngine::Vector4 operator*(float s, const KochaEngine::Vector4& v)
{
	return v * s;
}

inline const KochaEngine::Vector4 operator/(const KochaEngine::Vector4& v, float s)
{
	KochaEngine::Vector4 temp(v);
	return temp /= s;
}
