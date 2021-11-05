#pragma once
#include "Vector2.h"

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
	const Vector2 TALK_TEXT_POS = Vector2(220, 565);

	int KochaEngine::GameSetting::talkSpeed = 5;
	bool KochaEngine::GameSetting::isBattleField = false;
	int KochaEngine::GameSetting::nowField = GameSetting::HAZIMARINOTI;
}