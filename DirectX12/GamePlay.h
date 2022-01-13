#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "GameSetting.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class EffectManager;
	class Text;
	class Number;
	class Number3D;
	class Audio;
	class Texture2D;
	class Number3DManager;
	class Number3DEmitter;
	class Dx12_Wrapper;
	class CameraManager;

	class GamePlay : public Scene
	{
		enum CommandTab
		{
			DEFAULT_TAB,
			TARGET_SELECT_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:
		Dx12_Wrapper& dx12;

		Map* map;
		Camera* camera;
		CameraManager* cameraManager;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* pEmitter;
		EffectManager* effectManager;
		Number3DManager* n3DManager;
		Number3DEmitter* n3DEmitter;
		LightManager* lightManager;
		BattleObject* currentActiveActor;
		BattleObject* targetActor;

		Object* floor;
		Object* skyObj;
		Object* house;

		Texture2D* fadeTexture;
		Texture2D* defaultWakuTexture;
		Texture2D* anotherWakuTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* waitCommandTexture;
		Texture2D* anotherCommandTexture;
		Texture2D* pageCommandTexture;
		Texture2D* spCommandTexture;
		Texture2D* cursorTexture;

		Text* battleLongText;
		Text* battleShortText;
		Text* summaryText;
		Text* battleNameText;
		Text* commandTitleText;
		Text* enemyNameText[MAX_NAME_TEXT_COUNT_COMMAND];
		Text* skillNameText[MAX_NAME_TEXT_COUNT_COMMAND];

		Number* defaultNumberTex;
		Number* costSPNumberTex;
		Number* pageNumberTex;
		Number* maxPageNumberTex;

		int extraNum;

		//現在のコマンドタブ
		CommandTab currentTab;
		//直前のコマンドタブ
		CommandTab previousTab;

		//カーソルのポジション
		Vector2 cursorPos;

		//フェードアウトするかどうか
		bool fadeFlag;
		//バトル中かどうか
		bool isBattle;
		//バトルが終わっているかどうか
		bool isBattleEnd;
		//バトル開始の瞬間かどうか
		bool isBattleStart;
		//コマンド上の表示名を更新するかどうか
		bool isCommandTitleUpdate;
		//次のターンに行くかどうか
		bool isTurnUpdate;
		//モーション中かどうか
		bool isActiveMotion;
		//ぼうぎょモーション中かどうか
		bool isDefenceMotion;
		//敵のターンかどうか
		bool isEnemyTurn;
		//キャラクターが全滅しているかどうか
		bool isCharacterDestroy;
		//敵が全滅しているかどうか
		bool isEnemyDestroy;
		//リザルト時に一度だけ行う処理用フラグ
		bool isResultOnce;
		//デフォルトウィンドウに数字を描画するか
		bool isShowNumber;
		//コマンド説明文更新用
		bool isUpdateCommandExplanation;

		//フェードの透明度
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;
		//行動時のアニメーションの時間	
		int motionTime;
		//バトル参加キャラの数
		int battleCharaCount;
		//バトルスタート時の硬直時間
		int battleStartWait;
		//リザルト進行の番号
		int resultFlowNum;
		//リザルトでボタンを押したときの待ち時間
		int resultFlowWait;
		//獲得経験値
		int getExp;
		//選択中のスキル番号
		int selectSkillIndex;
		//スキルタブのページ番号
		int skillTabPageNum;
		//スキルタブの最大ページ番号
		int skillTabMaxPageNum;
		//スキルタブの直前のページ番号
		int preSkillTabPageNum;
		//スキルの消費SP
		int costSP;

		//コマンド番号
		unsigned int commandNum;
		//コマンド番号保存用 : 0,デフォルト
		unsigned int preCommandNum;


		void BattleInitialize();
		void BattleUpdate();
		void BattleFlowUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();
		void BattleSpriteDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();

		void BattleEnd();
		void TurnInitialize();
		void ActiveActorUpdate();
		void EnemyActionSelect();
		void ActiveMotionUpdate();
		void DefenceMotionUpdate();
		void ResultUpdate();
		void RewardCalc();
		void EnemyNameUpdate();
		void SkillNameUpdate();
		void SkillExplanationUpdate();
		void CommandDraw();
		void MoveCursor();
		void CursorPosSetting();
		void DefaultTab();
		void TargetSelectTab();
		void SkillTab();
		unsigned int CommandNumUp(const unsigned int arg_commandNum);
		unsigned int CommandNumDown(const unsigned int arg_commandNum);
		unsigned int CommandNumRight(const unsigned int arg_commandNum, const unsigned int arg_maxNum);
		unsigned int CommandNumLeft(const unsigned int arg_commandNum, const unsigned int arg_maxNum);

	public:
		GamePlay(Dx12_Wrapper& arg_dx12);
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
