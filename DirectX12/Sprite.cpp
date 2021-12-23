#include "Sprite.h"

KochaEngine::Sprite::Sprite(std::string texName, Vector2 position, Vector2 size, float rotate)
{
	tex = new Texture2D(texName, position, size, rotate);
	this->texName = texName;
	this->position = position;
	this->size = size;
	this->rotate = rotate;
}

KochaEngine::Sprite::~Sprite()
{
	delete tex;
}

void KochaEngine::Sprite::Draw()
{
	tex->Draw();
}

void KochaEngine::Sprite::Draw(const Vector2& arg_position, const Vector4& arg_color)
{
	tex->SetColor(arg_color);
	tex->Draw(arg_position);
}
