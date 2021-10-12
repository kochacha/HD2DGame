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
		void Draw(Vector2 position, DirectX::XMFLOAT4 color);
	};
}