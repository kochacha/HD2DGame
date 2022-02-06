#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	class GameSetting
	{
	public:

		//マップデータロード用番号
		enum
		{
			//最初のフィールド
			HAZIMARINOTI,
			//最初の町
			HAZIMARINOMATI,
		};

		//ゲームの更新スピード
		static double gameSpeed;

		//会話の流れるスピード
		static int talkSpeed;
		//敵とエンカウントするフィールドにいるかどうか
		static bool isBattleField;
		//現在のフィールド
		static int nowField;
		
		//サウンドボリューム関連
		static float masterVolume;
		static float seVolume;
		static float bgmVolume;

		//環境設定更新用
		static bool isEnvironmentUpdate;
		static int environmentNumber;
	};

#define MAX_BATTLE_CHARA 3
#define MAX_NAME_TEXT_COUNT_COMMAND 5

	//ゲーム画面サイズ
	const Vector2 SCREEN_SIZE = Vector2(1280, 720);

	//カメラ挙動
	const float BATTLE_FOCUS_EYE_Z = -55;
	const float BATTLE_FOCUS_TARGET_Z = 15;
	const float ATTACK_FOCUS_X = 15;

	//画面中央下の会話テキストの枠
	const Vector2 DEFAULT_WAKU_POS = Vector2(210, 570);
	const Vector2 ANOTHER_WAKU_POS = Vector2(298, 570);
	const Vector2 DEFAULT_WAKU_SIZE = Vector2(768, 96);
	const Vector2 ANOTHER_WAKU_SIZE = Vector2(680, 96);

	//バトル中コマンド
	const Vector2 DEFAULT_COMMAND_POS = Vector2(20, 460);
	const Vector2 DEFAULT_COMMAND_SIZE = Vector2(168, 208);
	const Vector2 ANOTHER_COMMAND_SIZE = Vector2(256, 208);
	const Vector2 ATTACK_COMMAND_SIZE = Vector2(256, 208);
	const Vector2 SP_COMMAND_SIZE = Vector2(128, 40);

	const unsigned int MAX_COMMAND_NUM = 4;

	//バトル中ステータス
	const Vector2 BATTLE_STATUS_POS = Vector2(1000, 500);
	const Vector2 BATTLE_STATUS_SIZE = Vector2(256, 178);
	const Vector2 MAX_GAUGE_SIZE = Vector2(235, 5);

	//画面中央下の会話テキスト
	const Vector2 TALK_LONG_TEXT_POS = Vector2(225, 585);
	const Vector2 TALK_SHORT_TEXT_POS = Vector2(313, 585);

	//バトル時エネミーのサイズ
	const Vector3 SMALL_ENEMY_SIZE = Vector3(10, 10, 10);
	const Vector3 MEDIUM_ENEMY_SIZE = Vector3(20, 20, 20);
	const Vector3 LARGE_ENEMY_SIZE = Vector3(30, 30, 30);

	//テクスチャアニメーションレート
	const int DEFAULT_ANIMATION_RATE = 12;
	const int DASH_ANIMATION_RATE = 8;

	//攻撃時アニメーション時間
	const int ATTACK_MOTION_TIME = 180;
	//const int SPELL_MOTION_TIME = 300;
	//const int SKILL_MOTION_TIME = 240;

	const int FOCUS_MOTION_TIME = 120;
	const int ACTIVE_MOTION_TIME = 60;

	//リザルト時ボタン入力待ち時間
	const int RESULT_INPUT_WAIT = 15;

	//ベース(レベル1状態)となる必要経験値
	const int BASE_EXP = 10;

	//プレイヤーからの相対位置
	const float CAMERA_POS_Y = 28;
	const float CAMERA_POS_Z = -60;
	const float CAMERA_TARGET_POS_Y = -5;
	const float CAMERA_TARGET_POS_Z = 25;

	//バトル時敵出現ポジション
	//小サイズの敵
	const Vector3 SMALL_ENEMY_POS[4] =
	{ Vector3(-20, -CAMERA_POS_Y, 55),Vector3(-25, -CAMERA_POS_Y, 45),Vector3(-40, -CAMERA_POS_Y, 60),Vector3(-45, -CAMERA_POS_Y, 50) };
	//中サイズの敵、又は飛んでいる敵
	const Vector3 MEDIUM_ENEMY_POS[4] =
	{ Vector3(-20, -CAMERA_POS_Y + 5, 55),Vector3(-25, -CAMERA_POS_Y + 5, 45),Vector3(-40, -CAMERA_POS_Y + 5, 60),Vector3(-45, -CAMERA_POS_Y + 5, 50) };
	//大サイズの敵
	const Vector3 LARGE_ENEMY_POS[4] =
	{ Vector3(-20, -CAMERA_POS_Y + 10, 55),Vector3(-25, -CAMERA_POS_Y + 10, 45),Vector3(-40, -CAMERA_POS_Y + 10, 60),Vector3(-45, -CAMERA_POS_Y + 10, 50) };

	//バトル時キャラクター出現ポジション
	const Vector3 BATTLE_CHARACTOR_POS[4] =
	{ Vector3(80, -CAMERA_POS_Y, 46),Vector3(77, -CAMERA_POS_Y, 50),Vector3(74, -CAMERA_POS_Y, 54),Vector3(71, -CAMERA_POS_Y, 58) };

	//ポストエフェクト関連
	
	//ビネットカラー
	//const Vector4 DEFAULT_VIGNETTE_COLOR = Vector4();
}