#pragma once
#include "Vector2.h"

namespace KochaEngine
{
	class Texture2D;

	class Font
	{
	private:
		Texture2D* texture;
		Vector2 position;
		Vector2 fontSize;
		unsigned int texNum;

		const int ConvertAscii(const char arg_character);

	public:
		Font(const unsigned int arg_texNum, const Vector2& arg_position, const Vector2& arg_fontSize);
		~Font();

		void Draw();
		void SetChar(const unsigned int arg_texNum);

		inline void SetPosition(const Vector2& arg_position) { position = arg_position; }
		//inline void SetFontSize(const Vector2& arg_fontSize) { fontSize = arg_fontSize; }
	};
}