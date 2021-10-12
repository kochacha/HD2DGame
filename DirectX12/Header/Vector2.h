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
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};
}

const KochaEngine::Vector2 operator+(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2);
const KochaEngine::Vector2 operator-(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2);
const KochaEngine::Vector2 operator*(const KochaEngine::Vector2& v, float s);
const KochaEngine::Vector2 operator*(float s, const KochaEngine::Vector2& v);
const KochaEngine::Vector2 operator/(const KochaEngine::Vector2& v, float s);