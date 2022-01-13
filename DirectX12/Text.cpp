#include "Text.h"
#include "Font.h"
#include "CSVReader.h"
#include "Audio.h"
#include "GameSetting.h"

KochaEngine::Text::Text(const Vector2& arg_position, const Vector2& arg_fontSize, const unsigned int arg_addSpeed)
{
	position = arg_position;
	fontSize = arg_fontSize;

	se = new Audio();

	oneLineFonts = 23;
	SetText("default.txt");
}

KochaEngine::Text::Text(const std::string& arg_textName, const Vector2& arg_position, const Vector2& arg_fontSize, const unsigned int arg_addSpeed)
{
	position = arg_position;
	fontSize = arg_fontSize;

	se = new Audio();

	oneLineFonts = 23;
	SetText(arg_textName);
}

void KochaEngine::Text::Initialize()
{
	textDataSize = 0;
	addTextCount = 0;
	addCount = 0;
	isSkip = false;
	isPlayEndText = false;
	isSound = false;
}

void KochaEngine::Text::AddFont(Font* arg_font)
{
	//�t�H���g���ꕶ���ǉ�
	if (arg_font == nullptr) return;
	fonts.push_back(arg_font);
}

void KochaEngine::Text::AddText()
{
	int fixY = addTextCount / oneLineFonts; //�����_�ȉ��͐؂�̂Ă�

	Vector2 fixPosition = Vector2(fontSize.x * (addTextCount - oneLineFonts * fixY), fontSize.y * fixY);

	//������ǉ�
	AddFont(new Font(textData[addTextCount], position + fixPosition, fontSize));
}

void KochaEngine::Text::Clear()
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
	delete se;
	Clear();
}

void KochaEngine::Text::Draw(const int arg_addSpeed)
{
	//addSpeed�t���[�����Ƀt�H���g��ǉ����Ă���
	if (arg_addSpeed > 0 && !isSkip)
	{
		if (addCount < arg_addSpeed)
		{
			addCount++;
		}
		else if (addTextCount < textDataSize)
		{
			AddText(); //�P�������ǉ�

			if (isSound && textData[addTextCount] != 85)
			{
				//�T�E���h��炷
				se->PlayWave(sePath.c_str(), GameSetting::seVolume);
			}

			addTextCount++;
			addCount = 0;

			if (addTextCount >= textDataSize)
			{
				//�e�L�X�g�̕`�悪����
				isPlayEndText = true;
			}
		}
	}
	else
	{
		isPlayEndText = true;
		//addSpeed��0�Ȃ瓯���ɑS���`�悷��
		for (; addTextCount < textDataSize; addTextCount++)
		{
			AddText(); //�P�������ǉ�

			if (isSound && addTextCount == textDataSize)
			{
				//�T�E���h��炷
				se->PlayWave(sePath.c_str(), GameSetting::seVolume);
			}
		}
	}
	isSkip = false;

	auto end = fonts.end();
	for (auto it = fonts.begin(); it != end; ++it)
	{
		(*it)->Draw();
	}
}

void KochaEngine::Text::SetText(const std::string& arg_textName)
{
	Initialize();
	Clear();

	auto textName = "Resources/Text/" + arg_textName;

	CSVReader reader;
	reader.LoadCSV(0, textName);
	std::vector<std::vector<int>> tmp = reader.GetMapData(0);
	textDataSize = tmp[0].size();
	textData.resize(textDataSize);
	for (int i = 0; i != textDataSize; i++)
	{
		textData[i] = tmp[0][i];
	}
}

void KochaEngine::Text::Skip()
{
	isSkip = true;
}

void KochaEngine::Text::SetSound(const std::string& arg_sePath)
{
	sePath = arg_sePath;
	isSound = true;
}
