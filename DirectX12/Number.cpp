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
	int digits = Util::GetDigit(arg_num) - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = arg_num;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(baka / hoge + 1);
		baka = baka % hoge;
		hoge /= 10;
	}

	int startDigit = maxDigit - Util::GetDigit(arg_num);
	for (int i = startDigit; i < maxDigit; i++)
	{
		numbers[i]->Draw();
	}
}

void KochaEngine::Number::Draw2(const int arg_num)
{
	Init();
	int digits = Util::GetDigit(arg_num) - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = arg_num;

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

void KochaEngine::Number::Draw(const Vector2& arg_position, const int arg_num)
{
	Init();
	int digits = Util::GetDigit(arg_num) - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = arg_num;

	for (int i = 0; i <= digits; i++)
	{
		numbers[maxDigit - digits + i - 1]->SetTexNum(baka / hoge + 1);
		baka = baka % hoge;
		hoge /= 10;
	}

	for (int i = 0; i < maxDigit; i++)
	{
		numbers[i]->Draw({ arg_position.x + (size.x + 5.0f) * i,arg_position.y });
	}
}
