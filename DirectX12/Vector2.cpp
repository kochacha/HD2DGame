//#include "Vector2.h"
//
//KochaEngine::Vector2::Vector2() : Vector2(0, 0)
//{
//}
//
//KochaEngine::Vector2::Vector2(float x, float y) : XMFLOAT2(x, y)
//{
//}
//
//float KochaEngine::Vector2::length() const
//{
//	return sqrt(x * x + y * y);
//}
//
//KochaEngine::Vector2& KochaEngine::Vector2::normalize()
//{
//	float len = length();
//	if (len != 0)
//	{
//		return *this /= len;
//	}
//	return *this;
//}
//
//float KochaEngine::Vector2::dot(const Vector2& v) const
//{
//	return x * v.x + y * v.y;
//}
//
//KochaEngine::Vector2 KochaEngine::Vector2::operator+() const
//{
//	return *this;
//}
//
//KochaEngine::Vector2 KochaEngine::Vector2::operator-() const
//{
//	return Vector2(-x, -y);
//}
//
//KochaEngine::Vector2& KochaEngine::Vector2::operator+=(const Vector2& v)
//{
//	x += v.x;
//	y += v.y;
//	return *this;
//}
//
//KochaEngine::Vector2& KochaEngine::Vector2::operator-=(const Vector2& v)
//{
//	x -= v.x;
//	y -= v.y;
//	return *this;
//}
//
//KochaEngine::Vector2& KochaEngine::Vector2::operator*=(float s)
//{
//	x *= s;
//	y *= s;
//	return *this;
//}
//
//KochaEngine::Vector2& KochaEngine::Vector2::operator/=(float s)
//{
//	x /= s;
//	y /= s;
//	return *this;
//}
//
//const KochaEngine::Vector2 operator+(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2)
//{
//	KochaEngine::Vector2 temp(v1);
//	return temp += v2;
//}
//
//const KochaEngine::Vector2 operator-(const KochaEngine::Vector2& v1, const KochaEngine::Vector2& v2)
//{
//	KochaEngine::Vector2 temp(v1);
//	return temp -= v2;
//}
//
//const KochaEngine::Vector2 operator*(const KochaEngine::Vector2& v, float s)
//{
//	KochaEngine::Vector2 temp(v);
//	return temp *= s;
//}
//
//const KochaEngine::Vector2 operator*(float s, const KochaEngine::Vector2& v)
//{
//	return v * s;
//}
//
//const KochaEngine::Vector2 operator/(const KochaEngine::Vector2& v, float s)
//{
//	KochaEngine::Vector2 temp(v);
//	return temp /= s;
//}
