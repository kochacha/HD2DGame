#include "Number.h"
#include "Util.h"

KochaEngine::Number::Number(const Vector2& arg_position, const Vector2& arg_size, const int arg_maxDigit)
{
	this->maxDigit = arg_maxDigit;
	for (int i = 0; i < arg_maxDigit; i++)
	{
		numbers[i] = new Texture2D("Resources/Texture/Font/number.png", 10, 1, 1, Vector2(arg_position.x + (arg_size.x + 2.0f) * i,arg_position.y), arg_size, 0);
	}
}

KochaEngine::Number::~Number()
{
	for (int i = 0; i < maxDigit; i++)
	{
		delete numbers[i];
	}
}

void KochaEngine::Number::Init()
{
	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->SetTexNum(1);
	}
	
}

void KochaEngine::Number::SetColor(const Vector4& arg_color)
{
	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->SetColor(arg_color);
	}
}

void KochaEngine::Number::Draw(const int arg_num)
{
	Init();
	int maxNum = (int)std::pow(10, maxDigit) - 1;
	int preNum = arg_num;
	if (preNum > maxNum) preNum = maxNum;
	int digits = Util::GetDigit(preNum) - 1;
	int currentDigit = (int)std::pow(10, digits);
	int currentNum = preNum;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(currentNum / currentDigit + 1);
		currentNum = currentNum % currentDigit;
		currentDigit /= 10;
	}

	int startDigit = maxDigit - Util::GetDigit(preNum);
	for (int i = startDigit; i < maxDigit; i++)
	{
		numbers[i]->Draw();
	}
}

void KochaEngine::Number::DrawAllDigit(const int arg_num)
{
	Init();
	int maxNum = (int)std::pow(10, maxDigit) - 1;
	int preNum = arg_num;
	if (preNum > maxNum) preNum = maxNum;
	int digits = Util::GetDigit(preNum) - 1;
	int currentDigit = (int)std::pow(10, digits);
	int currentNum = preNum;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(currentNum / currentDigit + 1);
		currentNum = currentNum % currentDigit;
		currentDigit /= 10;
	}

	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->Draw();
	}
}

void KochaEngine::Number::DrawAllDigit(const Vector2& arg_position, const int arg_num)
{
	Init();
	int maxNum = (int)std::pow(10, maxDigit) - 1;
	int preNum = arg_num;
	if (preNum > maxNum) preNum = maxNum;
	int digits = Util::GetDigit(preNum) - 1;
	int currentDigit = (int)std::pow(10, digits);
	int currentNum = preNum;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(currentNum / currentDigit + 1);
		currentNum = currentNum % currentDigit;
		currentDigit /= 10;
	}

	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->Draw({ arg_position.x + (size.x + 5.0f) * i,arg_position.y });
	}
}
