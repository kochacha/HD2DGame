#pragma once
#include "Texture2D.h"

namespace KochaEngine
{
	class Sprite
	{
	private:
		Texture2D* tex;

		std::string texName;
		Vector2 position;
		Vector2 size;
		float rotate;

	public:
		Sprite(std::string texName, Vector2 position, Vector2 size, float rotate);
		~Sprite();

		void Draw();
		void Draw(const Vector2& arg_position, const Vector4& arg_color);
	};
}