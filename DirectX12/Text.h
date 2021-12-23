#pragma once
#include <vector>
#include <string>
#include "Vector2.h"

namespace KochaEngine
{
	class Font;
	class Audio;

	class Text
	{
	private:
		Audio* se;

		std::vector<Font*> fonts;
		std::vector<int> textData;

		std::string sePath;
		Vector2 position;
		Vector2 fontSize;
		unsigned int addCount;
		unsigned int textDataSize;
		unsigned int addTextCount;
		bool isSkip;
		bool isPlayEndText;
		bool isSound;

		unsigned short oneLineFonts;

		void Initialize();
		void AddFont(Font* arg_font);
		void AddText();
		void Clear();

	public:
		Text(const Vector2& arg_position,
			const Vector2& arg_fontSize, const unsigned int arg_addSpeed = 5);
		Text(const std::string& arg_textName, const Vector2& arg_position,
			const Vector2& arg_fontSize, const unsigned int arg_addSpeed = 5);
		~Text();

		void Draw(const int arg_addSpeed);
		void SetText(const std::string& arg_textName);
		void Skip();

		inline const bool IsPlayEnd() { return isPlayEndText; }
		inline void SetPosition(const Vector2& arg_position) { position = arg_position; }
		inline void SetOneLineFonts(const unsigned short arg_oneLineFonts) { oneLineFonts = arg_oneLineFonts; }
		void SetSound(const std::string& arg_sePath);
	};

}