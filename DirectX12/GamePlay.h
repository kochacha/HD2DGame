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
	class Map;
	class LightManager;

	class GamePlay : public Scene
	{
	private:
		Map* map;
		Camera* camera;
		GameObjectManager* gManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;
		LightManager* lightManager;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;

		bool fadeFlag;
		bool isBattle;
		bool isBattleEnd;
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;

		void BattleUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();
		void BattleSpriteDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();
	public:
		GamePlay();
		~GamePlay() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void AlphaObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
		void CountTime();
		void FadeUpdate();
	};
}
