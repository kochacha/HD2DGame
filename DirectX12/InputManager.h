#pragma once

namespace KochaEngine
{
	class InputManager
	{
	public:
		//�ړ��֘A
		static bool MoveUp();
		static bool MoveDown();
		static bool MoveRight();
		static bool MoveLeft();
		static bool MoveDash();

		//�R�}���h�I���֘A
		static bool TriggerUp();
		static bool TriggerDown();
		static bool TriggerRight();
		static bool TriggerLeft();
		static bool TriggerDecision(); //����{�^��
		static bool TriggerCancel(); //�L�����Z���{�^��
	};
}