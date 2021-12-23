#pragma once
#include "Texture2D.h"

#define MAX_DIGIT 9

namespace KochaEngine
{
	class Number
	{
	private:
		Texture2D* numbers[MAX_DIGIT];

		Vector2 position;
		Vector2 size;
		int maxDigit;

	public:
		Number(const Vector2& arg_position, const Vector2& arg_size, const int arg_maxDigit);
		~Number();

		void Init();

		void SetColor(const Vector4& arg_color);

		void Draw(const int arg_num);
		void DrawAllDigit(const int arg_num);
		void DrawAllDigit(const Vector2& arg_position, const int arg_num);
	};
}