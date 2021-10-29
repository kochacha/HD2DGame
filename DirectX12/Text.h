#pragma once
#include <vector>
#include <string>
#include "Vector2.h"

namespace KochaEngine
{
	class Font;

	class Text
	{
	private:
		std::vector<Font*> fonts;
		std::vector<int> textData;

		Vector2 position;
		Vector2 fontSize;
		unsigned int count;
		unsigned int textDataSize;
		unsigned int addTextCount;

		const unsigned short MAX_ONE_LINE_FONTS = 30;

		void Initialize();
		void AddFont(Font* arg_font);
		void RemoveAll();

	public:
		Text(const std::string& arg_textName, const Vector2& arg_position,
			const Vector2& arg_fontSize, const unsigned int arg_addSpeed = 5);
		~Text();

		void Draw(const int arg_addSpeed);

		inline void SetPosition(const Vector2& arg_position) { position = arg_position; }
	};

}