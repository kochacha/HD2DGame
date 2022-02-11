#include "InputManager.h"
#include "Input.h"

//bool KochaEngine::InputManager::MoveUp()
//{
//    return Input::CheckKey(DIK_W) 
//        || Input::CheckPadLStickUp();
//}
//
//bool KochaEngine::InputManager::MoveDown()
//{
//    return Input::CheckKey(DIK_S) 
//        || Input::CheckPadLStickDown();
//}
//
//bool KochaEngine::InputManager::MoveRight()
//{
//    return Input::CheckKey(DIK_D) 
//        || Input::CheckPadLStickRight();
//}
//
//bool KochaEngine::InputManager::MoveLeft()
//{
//    return Input::CheckKey(DIK_A) 
//        || Input::CheckPadLStickLeft();
//}
//
//bool KochaEngine::InputManager::MoveDash()
//{
//    return Input::CheckKey(DIK_LSHIFT) 
//        || Input::CheckPadButton(XINPUT_GAMEPAD_X);
//}

bool KochaEngine::InputManager::TriggerUp()
{
    return Input::TriggerKey(DIK_W)
        || Input::TriggerKey(DIK_UP)
        || Input::TriggerPadLStickUp()
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP);
}

bool KochaEngine::InputManager::TriggerDown()
{
    return Input::TriggerKey(DIK_S)
        || Input::TriggerKey(DIK_DOWN)
        || Input::TriggerPadLStickDown()
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN);
}

bool KochaEngine::InputManager::TriggerRight()
{
    return Input::TriggerKey(DIK_D)
        || Input::TriggerKey(DIK_RIGHT)
        || Input::TriggerPadLStickRight()
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT);
}

bool KochaEngine::InputManager::TriggerLeft()
{
    return Input::TriggerKey(DIK_A)
        || Input::TriggerKey(DIK_LEFT)
        || Input::TriggerPadLStickLeft()
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT);
}

bool KochaEngine::InputManager::TriggerDecision()
{
    return Input::TriggerKey(DIK_Z)
        || Input::TriggerKey(DIK_RETURN)
        || Input::TriggerKey(DIK_SPACE)
        || Input::TriggerPadButton(XINPUT_GAMEPAD_A);
}

bool KochaEngine::InputManager::TriggerCancel()
{
    return Input::TriggerKey(DIK_X)
        || Input::TriggerKey(DIK_BACKSPACE)
        || Input::TriggerPadButton(XINPUT_GAMEPAD_B);
}
