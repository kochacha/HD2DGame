//#include "Vector4.h"
//
//KochaEngine::Vector4::Vector4()
//	:Vector4(0, 0, 0, 0)
//{
//}
//
//KochaEngine::Vector4::Vector4(float x, float y, float z, float w)
//	: XMFLOAT4(x, y, z, w)
//{
//}
//
//void KochaEngine::Vector4::Zero()
//{
//	x = y = z = w = 0;
//}
//
//KochaEngine::Vector4 KochaEngine::Vector4::operator+() const
//{
//	return *this;
//}
//
//KochaEngine::Vector4 KochaEngine::Vector4::operator-() const
//{
//	return Vector4(-x, -y, -z, -w);
//}
//
//KochaEngine::Vector4& KochaEngine::Vector4::operator+=(const Vector4& v)
//{
//	x += v.x;
//	y += v.y;
//	z += v.z;
//	w += v.w;
//	return *this;
//}
//
//KochaEngine::Vector4& KochaEngine::Vector4::operator-=(const Vector4& v)
//{
//	x -= v.x;
//	y -= v.y;
//	z -= v.z;
//	w -= v.w;
//	return *this;
//}
//
//KochaEngine::Vector4& KochaEngine::Vector4::operator*=(float s)
//{
//	x *= s;
//	y *= s;
//	z *= s;
//	w *= s;
//	return *this;
//}
//
//KochaEngine::Vector4& KochaEngine::Vector4::operator/=(float s)
//{
//	x /= s;
//	y /= s;
//	z /= s;
//	w /= s;
//	return *this;
//}
//
//const KochaEngine::Vector4 operator+(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2)
//{
//	KochaEngine::Vector4 temp(v1);
//	return temp += v2;
//}
//
//const KochaEngine::Vector4 operator-(const KochaEngine::Vector4& v1, const KochaEngine::Vector4& v2)
//{
//	KochaEngine::Vector4 temp(v1);
//	return temp -= v2;
//}
//
//const KochaEngine::Vector4 operator*(const KochaEngine::Vector4& v, float s)
//{
//	KochaEngine::Vector4 temp(v);
//	return temp *= s;
//}
//
//const KochaEngine::Vector4 operator*(float s, const KochaEngine::Vector4& v)
//{
//	return v * s;
//}
//
//const KochaEngine::Vector4 operator/(const KochaEngine::Vector4& v, float s)
//{
//	KochaEngine::Vector4 temp(v);
//	return temp /= s;
//}
