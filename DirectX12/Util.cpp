#include "Util.h"
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

	// �񌈒�_�I�ȗ�����������g�p����
	std::random_device engine;

	// �p�X���[�h���ƂȂ镶���W���͈̔͂���l���z������
	std::uniform_int_distribution<std::size_t> dist(0, candidate_chars.size() - 1);

	std::string password;
	for (std::size_t i = 0; i < length; ++i) {
		// �p�X���[�h���̕����W������A�����_���Ȉꕶ����I������
		std::size_t random_index = dist(engine);
		char random_char = candidate_chars[random_index];

		// �I�������������A�p�X���[�h������ɒǉ�����
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
