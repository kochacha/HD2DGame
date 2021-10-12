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
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};
}

const KochaEngine::Vector4 operator+(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2);
const KochaEngine::Vector4 operator-(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2);
const KochaEngine::Vector4 operator*(const KochaEngine::Vector4& v, float s);
const KochaEngine::Vector4 operator*(float s, const KochaEngine::Vector4& v);
const KochaEngine::Vector4 operator/(const KochaEngine::Vector4& v, float s);