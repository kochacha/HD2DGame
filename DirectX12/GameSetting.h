#pragma once

namespace KochaEngine
{
	class GameSetting
	{
		enum NextField
		{
			HAZIMARINOTI,
			HAZIMARINOMATI,
		};
	public:
		static int talkSpeed;
		static bool isBattleField;
	};

	int KochaEngine::GameSetting::talkSpeed = 5;
	bool KochaEngine::GameSetting::isBattleField = false;
}