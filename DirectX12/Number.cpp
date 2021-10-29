#include "Number.h"
#include "Util.h"

KochaEngine::Number::Number(Vector2 position, Vector2 size, int maxDigit)
{
	this->maxDigit = maxDigit;
	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i] = new Texture2D("Resources/Texture/Font/number.png", 10, 1, 1, Vector2(position.x + (size.x + 5.0f) * i,position.y), size, 0);
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

void KochaEngine::Number::Draw(int num)
{
	int digits = Util::GetDigit(num) - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = num;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(baka / hoge + 1);
		baka = baka % hoge;
		hoge /= 10;
	}

	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->Draw();
	}
}

void KochaEngine::Number::Draw(Vector2 position, int num)
{
	int digits = Util::GetDigit(num) - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = num;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(baka / hoge + 1);
		baka = baka % hoge;
		hoge /= 10;
	}

	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->Draw({ position.x + (size.x + 5.0f) * i,position.y });
	}
}
