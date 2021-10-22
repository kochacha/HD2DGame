#include "Text.h"
#include "Font.h"
#include "CSVReader.h"

KochaEngine::Text::Text(const std::string& arg_textName, const Vector2& arg_position, const Vector2& arg_fontSize, const unsigned int arg_addSpeed)
{
	CSVReader reader;
	reader.LoadCSV(0, arg_textName);
	std::vector<std::vector<int>> tmp = reader.GetMapData(0);
	textDataSize = tmp[0].size();
	textData.resize(textDataSize);
	for (int i = 0; i != textDataSize; i++)
	{
		textData[i] = tmp[0][i];
	}
	Initialize();
}

void KochaEngine::Text::Initialize()
{
	addTextCount = 0;
	count = 0;
}

void KochaEngine::Text::AddFont(Font* arg_font)
{
	fonts.push_back(arg_font);
}

KochaEngine::Text::~Text()
{
}

void KochaEngine::Text::Draw()
{
	if (count < 5)
	{
		count++;
	}
	else
	{
		if (addTextCount < textDataSize)
		{
			AddFont(new Font(textData[addTextCount], position + Vector2(32 * addTextCount, 0), Vector2(32, 32)));
		}
		count = 0;
	}

	auto end = fonts.end();
	for (auto it = fonts.begin(); it != end; ++it)
	{
		(*it)->Draw();
	}
}
