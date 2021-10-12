#pragma once
#include <DirectXMath.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define rad2deg(a) ( (a) / M_PI * 180.0 )

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 & lhs, const DirectX::XMFLOAT3 & rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3 & lhs, const DirectX::XMFLOAT3 & rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	result.z = lhs.z * rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3 & lhs, const DirectX::XMFLOAT3 & rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	result.z = lhs.z / rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3 & lhs, const float & rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	return result;
}

const DirectX::XMFLOAT3 operator+=(const DirectX::XMFLOAT3 & lhs, const DirectX::XMFLOAT3 & rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

DirectX::XMVECTOR AngleToVector2(float angle)
{
	DirectX::XMVECTOR result;
	float radian = angle * (M_PI / 180);
	result = { cos(radian), 0,sin(radian) };
	DirectX::XMVector3Normalize(result);
	return result;
}

float Vector2ToAngle(DirectX::XMFLOAT3 vector)
{
	float angle;
	angle = acos(vector.z / sqrt(vector.z*vector.z + vector.x * vector.x));
	angle = angle * 180.0 / M_PI;
	if (vector.x < 0) angle = 360.0f - angle;
	return angle;
}

