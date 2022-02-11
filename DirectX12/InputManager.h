#pragma once

namespace KochaEngine
{
	class InputManager
	{
	public:
		////移動関連
		//static bool MoveUp();
		//static bool MoveDown();
		//static bool MoveRight();
		//static bool MoveLeft();
		//static bool MoveDash();

		//コマンド選択関連
		static bool TriggerUp();
		static bool TriggerDown();
		static bool TriggerRight();
		static bool TriggerLeft();
		static bool TriggerDecision(); //決定ボタン
		static bool TriggerCancel(); //キャンセルボタン
	};
}