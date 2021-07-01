#pragma once
#include <string>
#include <cassert>
#include <codecvt>
#include <DirectXMath.h>

namespace KochaEngine
{
	class Util
	{
	public:
		//randomŠÖ”
		static int GetRandInt();
		static int GetRandInt(const unsigned int max);
		static int GetIntRand(const int minValue, const int maxValue);
		static std::string CreatePassword(const std::string& candidate_chars, std::size_t length);

		//easingŠÖ”
		static float Lerp(const float start, const float end, const float t);
		static float EaseIn(const float start, const float end, const float t);
		static float EaseOut(const float start, const float end, const float t);
		static float EaseInOut(const float start, const float end, const float t);

		//•¶š—ñŒ^•ÏŠ·ŠÖ”
		static std::string WstringToString(std::wstring wstring);
		static std::wstring StringToWstring(std::string string);
		static wchar_t* StringToWchar_t(std::string string);

		static float Length(DirectX::XMFLOAT3 a);
		static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 a);

		static DirectX::XMVECTOR AngleToVector2(float angle);


		static float Vector2ToAngle(DirectX::XMFLOAT3 vector);

		//Œ…”‚ğ“¾‚é
		static unsigned GetDigit(unsigned num);
	};
}
