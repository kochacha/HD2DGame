#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")

#define Release(X){if((X) != nullptr)(X)->Release();(X) = nullptr;}
#define STICKMAX 32767
#define DEADZONE 15000

HRESULT result;
LPDIRECTINPUT8 dinput = nullptr;
LPDIRECTINPUTDEVICE8 key = nullptr;
BYTE keys[KEY_MAX] = {};
BYTE olds[KEY_MAX] = {};
XINPUT_STATE padState;
XINPUT_STATE prevPadState;
LPDIRECTINPUTDEVICE8 devMouse = nullptr;
DIMOUSESTATE2 mouseState;
DIMOUSESTATE2 mouseStatePre;
XINPUT_VIBRATION vibration;
int time = 0;

void KochaEngine::Input::Init(HWND hwnd)
{
	memset(&keys, 0, sizeof(keys));
	memset(&olds, 0, sizeof(olds));

	result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&dinput), NULL);
	result = dinput->CreateDevice(GUID_SysKeyboard, &key, NULL);
	result = key->SetDataFormat(&c_dfDIKeyboard);
	result = key->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY);
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	result = devMouse->SetDataFormat(&c_dfDIMouse2);
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	key->Acquire();
	ZeroMemory(&padState, sizeof(XINPUT_STATE));
	ZeroMemory(&prevPadState, sizeof(XINPUT_STATE));
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
}

void KochaEngine::Input::Update()
{
	prevPadState = padState;
	XInputGetState(0, &padState);
	if (time > -1) { time--; };
	if (time <= 0)
	{
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
	}
	XInputSetState(0, &vibration);

	// マウス動作開始
	result = devMouse->Acquire();
	// 前回の入力を保存
	mouseStatePre = mouseState;
	// マウスの入力
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

void KochaEngine::Input::Terminate()
{
	Release(key);
	Release(dinput);
	Release(devMouse);
}

bool KochaEngine::Input::CheckKey(UINT index)
{
	bool flag = false;
	key->GetDeviceState(sizeof(keys), &keys);
	if (keys[index] & 0x80)
	{
		flag = true;
	}
	olds[index] = keys[index];
	return flag;
}

bool KochaEngine::Input::TriggerKey(UINT index)
{
	bool flag = false;
	key->GetDeviceState(sizeof(keys), &keys);
	if ((keys[index] & 0x80) && !(olds[index] & 0x80))
	{
		flag = true;
	}
	olds[index] = keys[index];
	return flag;
}

bool KochaEngine::Input::TriggerReleaseKey(UINT index)
{
	bool flag = false;
	key->GetDeviceState(sizeof(keys), &keys);
	if (!(keys[index] & 0x80) && (olds[index] & 0x80))
	{
		flag = true;
	}
	olds[index] = keys[index];
	return flag;
}

bool KochaEngine::Input::PushMouseLeft()
{
	if (mouseState.rgbButtons[0]) {
		return true;
	}
	return false;
}

bool KochaEngine::Input::PushMouseRight()
{
	if (mouseState.rgbButtons[1]) {
		return true;
	}
	return false;
}

bool KochaEngine::Input::PushMouseMiddle()
{
	if (mouseState.rgbButtons[2]) {
		return true;
	}
	return false;
}

bool KochaEngine::Input::TriggerMouseLeft()
{
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}
	return false;
}

bool KochaEngine::Input::TriggerMouseRight()
{
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1]) {
		return true;
	}
	return false;
}

bool KochaEngine::Input::TriggerMouseMiddle()
{
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}
	return false;
}

KochaEngine::Input::MouseMove KochaEngine::Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

bool KochaEngine::Input::CheckPadButton(int button)
{
	return padState.Gamepad.wButtons & button;
}

bool KochaEngine::Input::TriggerPadButton(int button)
{
	return (padState.Gamepad.wButtons & button) && !(prevPadState.Gamepad.wButtons & button);
}

bool KochaEngine::Input::CheckPadRightTrigger()
{
	return padState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool KochaEngine::Input::CheckPadLeftTrigger()
{
	return padState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool KochaEngine::Input::TriggerPadRightTrigger()
{
	return (padState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		!(prevPadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
}

bool KochaEngine::Input::TriggerPadLeftTrigger()
{
	return (padState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		!(prevPadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
}

bool KochaEngine::Input::CheckPadRStickRight()
{
	return padState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool KochaEngine::Input::CheckPadRStickLeft()
{
	return padState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool KochaEngine::Input::CheckPadRStickUp()
{
	return padState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool KochaEngine::Input::CheckPadRStickDown()
{
	return padState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool KochaEngine::Input::TriggerPadRStickRight()
{
	return (padState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadRStickLeft()
{
	return (padState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadRStickUp()
{
	return (padState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadRStickDown()
{
	return (padState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool KochaEngine::Input::CheckPadLStickRight()
{
	return padState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

bool KochaEngine::Input::CheckPadLStickLeft()
{
	return padState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

bool KochaEngine::Input::CheckPadLStickUp()
{
	return padState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

}

bool KochaEngine::Input::CheckPadLStickDown()
{
	return padState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

}

bool KochaEngine::Input::TriggerPadLStickRight()
{
	return (padState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadLStickLeft()
{
	return (padState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadLStickUp()
{
	return (padState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool KochaEngine::Input::TriggerPadLStickDown()
{
	return (padState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

KochaEngine::Input::float2 KochaEngine::Input::GetLStickDirection()
{
	float2 vec;
	if ((padState.Gamepad.sThumbLY <  DEADZONE &&
		padState.Gamepad.sThumbLY > -DEADZONE) &&
		(padState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		padState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{		
		
		vec.y = padState.Gamepad.sThumbLY = 0;
		vec.x = padState.Gamepad.sThumbLX = 0;
	}
	else
	{
		vec.y = padState.Gamepad.sThumbLY;
		vec.x = padState.Gamepad.sThumbLX;
	}
	

	vec.x /= STICKMAX;
	vec.y /= STICKMAX;
	return vec;
}

KochaEngine::Input::float2 KochaEngine::Input::GetRStickDirection()
{
	float2 vec;
	if ((padState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 padState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(padState.Gamepad.sThumbRY <  DEADZONE &&
		 padState.Gamepad.sThumbRY > -DEADZONE))
	{
		vec.x = padState.Gamepad.sThumbRX = 0;
		vec.y = padState.Gamepad.sThumbRY = 0;
	}
	else
	{
		vec.x = padState.Gamepad.sThumbRX;
		vec.y = padState.Gamepad.sThumbRY;
	}

	vec.x /= STICKMAX;
	vec.y /= STICKMAX;
	return vec;
}


void KochaEngine::Input::LeftVibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wLeftMotorSpeed = power;
}

void KochaEngine::Input::RightVibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wRightMotorSpeed = power;
}

void KochaEngine::Input::Vibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wLeftMotorSpeed = power;
	vibration.wRightMotorSpeed = power;
}