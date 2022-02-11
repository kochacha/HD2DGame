#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "GameSetting.h"
#include <memory>

namespace KochaEngine
{
	class LightManager;
	class EffectManager;
	class CameraManager;
	class Texture2D;
	class Number;
	class Number3D;
	class Number3DManager;
	class Number3DEmitter;
	class Text;
	class Audio;

	class Battle
	{
		//コマンド操作時の各ページを列挙
		enum CommandTab
		{
			DEFAULT_TAB,
			TARGET_SELECT_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:

		//所有者はGamePlayクラス
		std::weak_ptr<Camera> camera; //カメラ
		std::weak_ptr<CameraManager> cameraManager; //カメラの管理クラス
		std::weak_ptr<GameObjectManager> gManager;
		std::weak_ptr<BattleObjectManager> bManager; //バトル時キャラ・エネミーを管理クラス
		std::weak_ptr<EffectManager> effectManager; //エフェクト関連の管理クラス
		std::weak_ptr<Number3DEmitter> n3DEmitter; //3D空間上に数字を生成するクラス
		std::weak_ptr<LightManager> lightManager; //ライトの管理クラス

		//ターゲットや現在行動中のキャラを格納しておくための箱
		BattleObject* currentActiveActor;
		BattleObject* targetActor;

		//バトル時に描画されるUI関連
		std::unique_ptr<Texture2D> defaultWakuTexture;
		std::unique_ptr<Texture2D> anotherWakuTexture;
		std::unique_ptr<Texture2D> defaultCommandTexture;
		std::unique_ptr<Texture2D> waitCommandTexture;
		std::unique_ptr<Texture2D> anotherCommandTexture;
		std::unique_ptr<Texture2D> pageCommandTexture;
		std::unique_ptr<Texture2D> spCommandTexture;
		std::unique_ptr<Texture2D> cursorTexture;

		//バトル時に描画されるテキスト関連
		std::unique_ptr<Text> battleLongText;
		std::unique_ptr<Text> battleShortText;
		std::unique_ptr<Text> summaryText;
		std::unique_ptr<Text> battleNameText;
		std::unique_ptr<Text> commandTitleText;
		std::unique_ptr<Text> enemyNameText[MAX_NAME_TEXT_COUNT_COMMAND];
		std::unique_ptr<Text> skillNameText[MAX_NAME_TEXT_COUNT_COMMAND];

		//UIとして表示される数字
		std::unique_ptr<Number> defaultNumberTex;
		std::unique_ptr<Number> costSPNumberTex;
		std::unique_ptr<Number> pageNumberTex;
		std::unique_ptr<Number> maxPageNumberTex;

		//現在のコマンドタブ
		CommandTab currentTab;
		//直前のコマンドタブ
		CommandTab previousTab;

		//カーソルのポジション
		Vector2 cursorPos;

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

		//バトル参加キャラの数
		int battleCharaCount;
		//行動時のアニメーションの時間	
		int motionTime;
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
		void BattleFlowUpdate();
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

		Battle(std::weak_ptr<Camera> arg_camera, std::weak_ptr<CameraManager> arg_cameraManager, 
			std::weak_ptr<GameObjectManager> arg_gManager, std::weak_ptr<BattleObjectManager> arg_bManager, 
			std::weak_ptr<EffectManager> arg_effectManager, std::weak_ptr<Number3DEmitter> arg_n3DEmitter,
			std::weak_ptr<LightManager> arg_lightManager);
		~Battle();
		void Initialize();
		void Update();
		void SpriteDraw();
		void ObjDraw();
		void AlphaObjDraw();
		void DrawGUI();
		void Load();

		void BattleEndUpdate();
		void IsBattleTrue() { isBattle = true; }
		void IsBattleFalse() { isBattle = false; }
		void IsBattleStartFalse() { isBattleStart = false; }
		inline bool IsFade() { return fadeFlag; }
		inline bool IsBattle() { return isBattle; }
		inline bool IsBattleStart() { return isBattleStart; }
		inline bool IsBattleEnd() { return isBattleEnd; }
	};
}
