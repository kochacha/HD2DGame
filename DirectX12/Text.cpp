#include "Text.h"
#include "Font.h"
#include "CSVReader.h"

KochaEngine::Text::Text(const std::string& arg_textName, const Vector2& arg_position, const Vector2& arg_fontSize, const unsigned int arg_addSpeed)
{
	position = arg_position;
	fontSize = arg_fontSize;

	Initialize();

	CSVReader reader;
	reader.LoadCSV(0, arg_textName);
	std::vector<std::vector<int>> tmp = reader.GetMapData(0);
	textDataSize = tmp[0].size();
	textData.resize(textDataSize);
	for (int i = 0; i != textDataSize; i++)
	{
		textData[i] = tmp[0][i];
	}
}

void KochaEngine::Text::Initialize()
{
	textDataSize = 0;
	addTextCount = 0;
	count = 0;
}

void KochaEngine::Text::AddFont(Font* arg_font)
{
	//フォントを一文字追加
	if (arg_font == nullptr) return;
	fonts.push_back(arg_font);
}

void KochaEngine::Text::RemoveAll()
{
	auto end = fonts.end();
	for (auto it = fonts.begin(); it != end; ++it)
	{
		delete (*it);
	}
	fonts.clear();
}

KochaEngine::Text::~Text()
{
	RemoveAll();
}

void KochaEngine::Text::Draw(const int arg_addSpeed)
{
	//addSpeedフレーム毎にフォントを追加していく
	if (arg_addSpeed > 0)
	{
		if (count < arg_addSpeed)
		{
			count++;
		}
		else if (addTextCount < textDataSize)
		{
			AddFont(new Font(textData[addTextCount], position + Vector2(fontSize.x * addTextCount, 0), fontSize));
			addTextCount++;
			count = 0;
		}
	}
	else
	{
		for (; addTextCount < textDataSize; addTextCount++)
		{
			AddFont(new Font(textData[addTextCount], position + Vector2(fontSize.x * addTextCount, 0), fontSize));
		}
	}

	auto end = fonts.end();
	for (auto it = fonts.begin(); it != end; ++it)
	{
		(*it)->Draw();
	}
}
