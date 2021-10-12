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

		float length() const;
		Vector3& normalize();
		float dot(const Vector3& v) const;
		Vector3 cross(const Vector3& v) const;

		Vector3 operator+() const;
		Vector3 operator-() const;

		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};
}

const KochaEngine::Vector3 operator+(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2);
const KochaEngine::Vector3 operator-(const KochaEngine::Vector3& v1, const KochaEngine::Vector3& v2);
const KochaEngine::Vector3 operator*(const KochaEngine::Vector3& v, float s);
const KochaEngine::Vector3 operator*(float s, const KochaEngine::Vector3& v);
const KochaEngine::Vector3 operator/(const KochaEngine::Vector3& v, float s);

const KochaEngine::Vector3 lerp(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
const KochaEngine::Vector3 easeIn(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
const KochaEngine::Vector3 easeOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);
const KochaEngine::Vector3 easeInOut(const KochaEngine::Vector3& start, const KochaEngine::Vector3& end, const float t);