#pragma once
#include<DirectXMath.h>

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
Quaternion quaternion(const DirectX::XMVECTOR& v, float angle);
Quaternion quaternion(const DirectX::XMFLOAT3& v, const Quaternion& q);

float dot(const Quaternion& q1, const Quaternion& q2);

float length(const Quaternion& q);

Quaternion normalize(const Quaternion& q);

Quaternion conjugate(const Quaternion& q);

Quaternion operator + (const Quaternion& q);
Quaternion operator - (const Quaternion& q);

Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);

Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q, float s);
Quaternion operator * (float s, const Quaternion& q);
Quaternion operator / (const Quaternion& q, float s);

Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t);

DirectX::XMFLOAT3 getAxis(const Quaternion& q);
