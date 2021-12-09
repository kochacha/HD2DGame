#pragma once
#include <dinput.h>
#include <Windows.h>
#include <Xinput.h>
#define DIRECTINPUT_VERSION 0x0800 
#define KEY_MAX 256

namespace KochaEngine
{
	static class Input
	{
	public:
		struct MouseMove {
			LONG lX;
			LONG lY;
			LONG lZ;
		};

		struct float2
		{
			float x;
			float y;
		};

	public:
		static void Init(const HWND arg_hwnd);
		static void Update();
		static void Terminate();
		static bool CheckKey(const UINT arg_index);
		static bool TriggerKey(const UINT arg_index);
		static bool TriggerReleaseKey(const UINT arg_index);

		//�}�E�X
		static bool PushMouseLeft();
		static bool PushMouseRight();
		static bool PushMouseMiddle();
		static bool TriggerMouseLeft();
		static bool TriggerMouseRight();
		static bool TriggerMouseMiddle();
		static MouseMove GetMouseMove();

		//�Q�[���p�b�h
		static bool CheckPadButton(const int arg_button);
		static bool TriggerPadButton(const int arg_button);

		static bool CheckPadRightTrigger(); //�E�g���K�[
		static bool CheckPadLeftTrigger();  //���g���K�[

		static bool TriggerPadRightTrigger(); //�E�g���K�[�̃g���K�[
		static bool TriggerPadLeftTrigger(); //���g���K�[�̃g���K�[

		//�E�X�e�B�b�N
		static bool CheckPadRStickRight();
		static bool CheckPadRStickLeft();
		static bool CheckPadRStickUp();
		static bool CheckPadRStickDown();

		static bool TriggerPadRStickRight();
		static bool TriggerPadRStickLeft();
		static bool TriggerPadRStickUp();
		static bool TriggerPadRStickDown();

		//���X�e�B�b�N
		static bool CheckPadLStickRight();
		static bool CheckPadLStickLeft();
		static bool CheckPadLStickUp();
		static bool CheckPadLStickDown();

		static bool TriggerPadLStickRight();
		static bool TriggerPadLStickLeft();
		static bool TriggerPadLStickUp();
		static bool TriggerPadLStickDown();

		static float2 GetLStickDirection();
		static float2 GetRStickDirection();

		//�Q�[���p�b�h�U��
		static void LeftVibration(const unsigned int arg_power, const int arg_setTime);
		static void RightVibration(const unsigned int arg_power, const int arg_setTime);
		static void Vibration(const unsigned int arg_power, const int arg_setTime);
	};
}