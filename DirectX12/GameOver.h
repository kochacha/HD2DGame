#pragma once
#include "Scene.h"

namespace KochaEngine
{
	class GameOver : public Scene
	{
	public:
		GameOver();
		~GameOver() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
	};
}
