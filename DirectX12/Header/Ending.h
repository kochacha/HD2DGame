#pragma once
#include "Scene.h"
#include "Texture2D.h"
#include "Audio.h"
#include "Number.h"
#include <vector>

namespace KochaEngine
{
	class Ending : public Scene
	{
	private:
		bool fadeFlag;
		bool nextFlag;

		float fadeAlpha;
		float down;

		int count;

		void Fade();

	public:
		Ending();
		~Ending() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
	};
}