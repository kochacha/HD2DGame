#include "../Header/Util.h"
#include <random>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

int KochaEngine::Util::GetRandInt()
{
	std::random_device random;
	unsigned int result = random();
	return result;
}

int KochaEngine::Util::GetRandInt(const unsigned int max)
{
	std::random_device random;
	unsigned int result = random() % max;
	return result;
}

int KochaEngine::Util::GetIntRand(const int minValue, const int maxValue)
{
	std::random_device rnd;
	std::mt19937_64 mt64(rnd());
	std::uniform_int_distribution<int> genRandFloat(minValue, maxValue);
	return genRandFloat(mt64);
}

std::string KochaEngine::Util::CreatePassword(const std::string & candidate_chars, std::size_t length)
{
	assert(!candidate_chars.empty());

	// 非決定論的な乱数生成器を使用する
	std::random_device engine;

	// パスワード候補となる文字集合の範囲を一様分布させる
	std::uniform_int_distribution<std::size_t> dist(0, candidate_chars.size() - 1);

	std::string password;
	for (std::size_t i = 0; i < length; ++i) {
		// パスワード候補の文字集合から、ランダムな一文字を選択する
		std::size_t random_index = dist(engine);
		char random_char = candidate_chars[random_index];

		// 選択した文字を、パスワード文字列に追加する
		password += random_char;
	}
	return password;
}

float KochaEngine::Util::Lerp(const float start, const float end, const float t)
{
	return start * (1.0f - t) + end * t;
}

float KochaEngine::Util::EaseIn(const float start, const float end, const float t)
{
	float y = t * t;
	return Lerp(start, end, y);
}

float KochaEngine::Util::EaseOut(const float start, const float end, const float t)
{
	float y = t * (2 - t);
	return Lerp(start, end, y);
}

float KochaEngine::Util::EaseInOut(const float start, const float end, const float t)
{
	float y = t * t * (3 - 2 * t);
	return Lerp(start, end, y);
}

std::string KochaEngine::Util::WstringToString(std::wstring wstring)
{

	return std::string();
}

std::wstring KochaEngine::Util::StringToWstring(std::string string)
{
	return std::wstring();
}

wchar_t* KochaEngine::Util::StringToWchar_t(std::string string)
{
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, wfilepath, _countof(wfilepath));
	return wfilepath;
}

float KochaEngine::Util::Length(DirectX::XMFLOAT3 a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

DirectX::XMFLOAT3 KochaEngine::Util::Normalize(DirectX::XMFLOAT3 a)
{
	float temp = Length(a);
	if (temp != 0)
	{
		return { a.x / temp, a.y / temp ,a.z / temp };
	}
	return a;
}
DirectX::XMVECTOR KochaEngine::Util::AngleToVector2(float angle)
{
	DirectX::XMVECTOR result;
	float radian = angle * (M_PI / 180);
	result = { cos(radian), 0,sin(radian) };
	DirectX::XMVector3Normalize(result);
	return result;
}

float KochaEngine::Util::Vector2ToAngle(DirectX::XMFLOAT3 vector)
{
	float angle;
	angle = acos(vector.z / sqrt(vector.z*vector.z + vector.x * vector.x));
	angle = angle * 180.0 / M_PI;
	if (vector.x < 0) angle = 360.0f - angle;
	return angle;
}

unsigned KochaEngine::Util::GetDigit(unsigned num)
{
	unsigned digit = 0;
	if (num == 0) digit = 1;
	while (num != 0) {
		num /= 10;
		digit++;
	}
	return digit;
}
