#pragma once
#include "Scene.h"
#include "Audio.h"
#include "Object.h"
#include "Camera.h"

namespace KochaEngine
{
	class Texture2D;
	class Title : public Scene
	{
	private:
		Object* obj;
		Texture2D* backTexture;
		Texture2D* fadeTexture;

		bool bgmFlag;
		bool fadeFlag;
		float fadeAlpha;
		float down;

		int count;

		void FadeUpdate();

	public:
		Title();
		~Title() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
	};
}