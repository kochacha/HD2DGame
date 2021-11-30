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
#include "GameSetting.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class Text;

	class GamePlay : public Scene
	{
		enum CommandTab
		{
			DEFAULT_TAB,
			ATTACK_TAB,
			SPELL_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:
		Map* map;
		Camera* camera;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;
		LightManager* lightManager;
		BattleObject* nowActiveActor;
		BattleObject* targetActor;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;
		Texture2D* defaultWakuTexture;
		Texture2D* anotherWakuTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* waitCommandTexture;
		Texture2D* anotherCommandTexture;
		Texture2D* cursorTexture;
		Text* battleText;
		Text* commandTitleText;
		Text* enemyNameText[MAX_BATTLE_ENEMY];

		CommandTab nowTab;

		Vector2 cursorPos;

		bool fadeFlag;
		bool isBattle;
		bool isBattleEnd;
		bool isBattleStart;
		bool isTextUpdate;
		bool isTurnUpdate;
		bool isAttackMotion;
		bool isDefenceMotion;
		bool isEnemyTurn;
		float fadeAlpha;

		int motionTime;
		int frameCount;
		int seconds;
		int endCount;
		int battleCharaCount;
		int battleStartWait;

		unsigned int commandNum;
		unsigned int preCommandNum[5];


		void BattleInitialize();
		void BattleUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();
		void BattleSpriteDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();

		void AttackMotionUpdate();
		void DefenceMotionUpdate();
		void EnemyNameUpdate();
		void CommandDraw();
		void MoveCursor();
		void CursorPosSetting();
		void DefaultTab();
		void AttackTab();
		unsigned int CommandNumUp(const unsigned int arg_commandNum);
		unsigned int CommandNumDown(const unsigned int arg_commandNum);

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
