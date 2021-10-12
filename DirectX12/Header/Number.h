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
		Number(Vector2 position, Vector2 size, int maxDigit);
		~Number();

		void Init();

		void SetColor(DirectX::XMFLOAT4 color);

		void Draw(int num);
		void Draw(Vector2 position, int num);
	};
}