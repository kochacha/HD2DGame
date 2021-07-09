#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Number.h"
#include "Audio.h"
#include "Texture2D.h"

namespace KochaEngine
{
	class GamePlay : public Scene
	{

	private:
		Camera* camera;
		GameObjectManager* gManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;

		bool fadeFlag;
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;
	public:

		GamePlay();
		~GamePlay() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
		void CountTime();
		void Fade();
	};
}
