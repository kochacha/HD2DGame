#include "Text.h"

KochaEngine::Text::Text(const char* arg_text, const Vector2& arg_position, const Vector2& arg_fontSize)
{
    position = arg_position;
    fontSize = arg_fontSize;

    const char* text = arg_text;
    //incrementSize = 10;//GetStrLength(text);
    //' 'としてすべて生成
    for (int i = 0; i < INCREMENT_SIZE; i++)
    {
        textures[i] = new Texture2D("Resources/Texture/font.png", 10, 7, 68, Vector2(position.x + (fontSize.x + 2.0f) * i, position.y), fontSize, 0);
    }
    
    SetText(text);
}

KochaEngine::Text::~Text()
{
    for (int i = 0; i < INCREMENT_SIZE; i++)
    {
        delete textures[i];
    }
}

void KochaEngine::Text::Draw()
{
    for (int i = 0; i < INCREMENT_SIZE; i++)
    {
        textures[i]->Draw();
    }
}

void KochaEngine::Text::SetLeadText(const int arg_texNum)
{
    textures[0]->SetTexNum(arg_texNum);
}

void KochaEngine::Text::SetText(const char* arg_text)
{
    bool isSpace = false;
    for (int i = 0; i < INCREMENT_SIZE; i++)
    {
        if (isSpace)
        {
            textures[i]->SetTexNum(68);
            continue;
        }
        //ここでコンバート
        int texNum = ConvertAscii(*(arg_text + i));
        if (*(arg_text + i) == '\0' || texNum < 1 || texNum > 70)
        {
            textures[i]->SetTexNum(68);
            isSpace = true;
        }
        else
        {
            textures[i]->SetTexNum(texNum);
        }

    }
}

const int KochaEngine::Text::ConvertAscii(const char arg_charcter)
{
    int n = arg_charcter;
    Vector2 returnValue;

    //アンダーバーかどうか
    if (n == 95)
    {
        return 67;
    }

    //数字かどうか
    if (n < 60)
    {
        return n - 47;
    }
    //大文字かどうか
    else if (n < 92)
    {
        n -= 65;
        return n % 10 + (n / 10) * 10 + 11;
    }
    else if (n < 123)
    {
        //小文字
        n -= 97;
        return n % 10 + (n / 10) * 10 + 41;
    }

    return 68;
}

const unsigned int KochaEngine::Text::GetStrLength(const char* arg_strBegin)
{
    unsigned int index = 0;
    while (*(arg_strBegin + index) != '\0')
    {
        index++;
    }
    return index;
}
