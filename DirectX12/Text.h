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
		bool isSkip;

		const unsigned short MAX_ONE_LINE_FONTS = 23;

		void Initialize();
		void AddFont(Font* arg_font);
		void RemoveAll();

	public:
		Text(const Vector2& arg_position,
			const Vector2& arg_fontSize, const unsigned int arg_addSpeed = 5);
		Text(const std::string& arg_textName, const Vector2& arg_position,
			const Vector2& arg_fontSize, const unsigned int arg_addSpeed = 5);
		~Text();

		void Draw(const int arg_addSpeed);
		void ReText(const std::string& arg_textName);
		void Skip();

		inline void SetPosition(const Vector2& arg_position) { position = arg_position; }
	};

}