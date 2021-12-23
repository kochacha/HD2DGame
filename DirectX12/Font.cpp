#include "Font.h"
#include "Texture2D.h"

KochaEngine::Font::Font(const unsigned int arg_texNum, const Vector2& arg_position, const Vector2& arg_fontSize)
{
    texNum = arg_texNum;
    position = arg_position;
    fontSize = arg_fontSize;

    texture = new Texture2D("Resources/Texture/Font/font_japanese.png", 10, 18, texNum, position, fontSize, 0);
}

KochaEngine::Font::~Font()
{
    delete texture;
}

void KochaEngine::Font::Draw()
{
    texture->Draw(position);
}

void KochaEngine::Font::SetChar(const unsigned int arg_texNum)
{
    texture->SetTexNum(arg_texNum);
}

const int KochaEngine::Font::ConvertAscii(const char arg_character)
{
    int n = arg_character;
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