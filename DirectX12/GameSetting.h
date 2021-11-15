#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace KochaEngine
{
	class GameSetting
	{
	public:
		enum
		{
			HAZIMARINOTI,
			HAZIMARINOMATI,
		};
		static int talkSpeed;
		static bool isBattleField;
		static int nowField;
	};

	//ゲーム画面サイズ
	const Vector2 SCREEN_SIZE = Vector2(1280, 720);

	//画面中央下の会話テキストの枠
	const Vector2 DEFAULT_WAKU_POS = Vector2(210, 550);
	const Vector2 DEFAULT_WAKU_SIZE = Vector2(768, 96);

	//バトル中コマンド
	const Vector2 DEFAULT_COMMAND_POS = Vector2(20, 450);
	const Vector2 DEFAULT_COMMAND_SIZE = Vector2(168, 240);

	//バトル中ステータス
	const Vector2 BATTLE_STATUS_POS = Vector2(1000, 500);
	const Vector2 BATTLE_STATUS_SIZE = Vector2(256, 178);

	//画面中央下の会話テキスト
	const Vector2 TALK_TEXT_POS = Vector2(225, 565);

	//バトル時エネミーのサイズ
	const Vector3 SMALL_ENEMY_SIZE = Vector3(10, 10, 10);
	const Vector3 MEDIUM_ENEMY_SIZE = Vector3(20, 20, 20);
	const Vector3 LARGE_ENEMY_SIZE = Vector3(30, 30, 30);

	//バトル時敵出現ポジション
	//小サイズの敵
	const Vector3 SMALL_ENEMY_POS[4] =
	{ Vector3(-20, -20, 70),Vector3(-25, -20, 50),Vector3(-40, -20, 80),Vector3(-45, -20, 60) };
	//中サイズの敵、又は飛んでいる敵
	const Vector3 MEDIUM_ENEMY_POS[4] =
	{ Vector3(-20, -15, 70),Vector3(-25, -15, 50),Vector3(-40, -15, 80),Vector3(-45, -15, 60) };
	//大サイズの敵
	const Vector3 LARGE_ENEMY_POS[4] =
	{ Vector3(-20, -10, 70),Vector3(-25, -10, 50),Vector3(-40, -10, 80),Vector3(-45, -10, 60) };

	//バトル時キャラクター出現ポジション
	const Vector3 BATTLE_CHARACTOR_POS[4] =
	{ Vector3(50, -20, 60),Vector3(50, -20, 60),Vector3(50, -20, 60),Vector3(50, -20, 60) };
}