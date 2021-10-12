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
		Camera* camera;
		Object* obj;

		bool bgmFlag;
		bool fadeFlag;
		float fadeAlpha;
		float down;

		int count;

		void Fade();

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