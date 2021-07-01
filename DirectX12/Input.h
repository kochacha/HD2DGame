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
		static void Init(HWND hwnd);
		static void Update();
		static void Terminate();
		static bool CheckKey(UINT index);
		static bool TriggerKey(UINT index);
		static bool TriggerReleaseKey(UINT index);

		//�}�E�X
		static bool PushMouseLeft();
		static bool PushMouseRight();
		static bool PushMouseMiddle();
		static bool TriggerMouseLeft();
		static bool TriggerMouseRight();
		static bool TriggerMouseMiddle();
		static MouseMove GetMouseMove();

		//�Q�[���p�b�h
		static bool CheckPadButton(int button);
		static bool TriggerPadButton(int button);

		static bool CheckPadRightTrigger(); //�E�g���K�[
		static bool CheckPadLeftTrigger();  //���g���K�[

		static bool TriggerPadRightTrigger(); //�E�g���K�[�̃g���K�[w
		static bool TriggerPadLeftTrigger(); //���g���K�[�̃g���K�[w

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
		static void LeftVibration(unsigned int power, int setTime);
		static void RightVibration(unsigned int power, int setTime);
		static void Vibration(unsigned int power, int setTime);
	};
}