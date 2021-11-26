#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Number.h"
#include "Audio.h"
#include "Texture2D.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class Text;

	class GamePlay : public Scene
	{
	private:
		Map* map;
		Camera* camera;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;
		LightManager* lightManager;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;
		Texture2D* defaultWakuTexture;
		Texture2D* battleStatusTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* attackCommandTexture;
		Texture2D* cursorTexture;
		Text* battleText;
		Text* commandTitleText;
		Text* enemyNameText[4];

		Vector2 cursorPos;

		bool fadeFlag;
		bool isBattle;
		bool isBattleEnd;
		bool isBattleStart;
		bool isTextUpdate;
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;

		unsigned int commandNum;

		void BattleUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();
		void BattleSpriteDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();

		void MoveCursor();
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
