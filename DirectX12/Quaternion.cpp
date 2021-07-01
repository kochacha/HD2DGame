#include "Quaternion.h"
#include<cmath>


Quaternion quaternion(float x, float y, float z, float w)
{
	Quaternion result{ x,y,z,w };
	return result;
}

Quaternion quaternion(const DirectX::XMVECTOR& vec, float angle)
{
	DirectX::XMFLOAT3 v;
	DirectX::XMStoreFloat3(&v, vec);
	float _sin = sin(angle / 2.0f);
	return quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
}

Quaternion quaternion(const DirectX::XMFLOAT3& v, const Quaternion & q)
{
	Quaternion qq = conjugate(q);
	Quaternion qv = quaternion(v.x, v.y, v.z, 0);

	return q * qv * qq;
}

float dot(const Quaternion & q1, const Quaternion & q2)
{
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

float length(const Quaternion & q)
{
	return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

Quaternion normalize(const Quaternion & q)
{
	Quaternion result = q;
	float len = length(result);
	if (len != 0)
	{
		result /= len;
	}
	return result;
}

Quaternion conjugate(const Quaternion & q)
{
	Quaternion result = { -q.x,-q.y,-q.z,q.w };
	return result;
}

Quaternion operator+(const Quaternion & q)
{
	return q;
}

Quaternion operator-(const Quaternion & q)
{
	Quaternion result = q;
	result.x = -result.x;
	result.y = -result.y;
	result.z = -result.z;
	result.w = -result.w;
	return result;
}

Quaternion & operator+=(Quaternion & q1, const Quaternion & q2)
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;
	return q1;
}

Quaternion & operator-=(Quaternion & q1, const Quaternion & q2)
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;
	return q1;
}

Quaternion & operator*=(Quaternion & q, float s)
{
	q.x *= s;
	q.y *= s;
	q.z *= s;
	q.w *= s;
	return q;
}

Quaternion & operator/=(Quaternion & q, float s)
{
	return q *= 1.0f / s;
}

Quaternion & operator*=(Quaternion & q1, const Quaternion & q2)
{
	Quaternion result{
		 q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		 q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w,
	};
	q1 = result;
	return q1;
}

Quaternion operator+(const Quaternion & q1, const Quaternion & q2)
{
	Quaternion result = q1;
	return result += q2;
}

Quaternion operator-(const Quaternion & q1, const Quaternion & q2)
{
	Quaternion result = q1;
	return result -= q2;
}

Quaternion operator*(const Quaternion & q1, const Quaternion & q2)
{
	Quaternion result = q1;
	return result *= q2;
}

Quaternion operator*(const Quaternion & q, float s)
{
	Quaternion result = q;
	return result *= s;
}

Quaternion operator*(float s, const Quaternion & q)
{
	Quaternion result
	{
		s *= q.x,
		s *= q.y,
		s *= q.z,
		s *= q.w,
	};
	return result;
}

Quaternion operator/(const Quaternion & q, float s)
{
	Quaternion result = q;
	return result /= s;
}

Quaternion slerp(const Quaternion & q1, const Quaternion & q2, float t)
{
	float cos = dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f)
	{
		float theta = (float)acos(cos);
		k0 = (float)(sin(theta * k0) / sin(theta));
		k1 = (float)(sin(theta * k1) / sin(theta));
	}
	return q1 * k0 + t2 * k1;
}

Quaternion lerp(const Quaternion & q1, const Quaternion & q2, float t)
{
	float cos = dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	return q1 * k0 + t2 * k1;
}





DirectX::XMFLOAT3 getAxis(const Quaternion & q)
{
	DirectX::XMFLOAT3 result;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float _len = length(q);
	result.x = x / _len;
	result.y = y / _len;
	result.z = z / _len;

	return result;
}
