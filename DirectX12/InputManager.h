#pragma once

namespace KochaEngine
{
	class InputManager
	{
	public:
		static bool MoveUp();
		static bool MoveDown();
		static bool MoveRight();
		static bool MoveLeft();
		static bool MoveDash();
	};
}