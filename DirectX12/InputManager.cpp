#include "InputManager.h"
#include "Input.h"

bool KochaEngine::InputManager::MoveUp()
{
    return Input::CheckKey(DIK_W) || Input::CheckPadLStickUp();
}

bool KochaEngine::InputManager::MoveDown()
{
    return Input::CheckKey(DIK_S) || Input::CheckPadLStickDown();
}

bool KochaEngine::InputManager::MoveRight()
{
    return Input::CheckKey(DIK_D) || Input::CheckPadLStickRight();
}

bool KochaEngine::InputManager::MoveLeft()
{
    return Input::CheckKey(DIK_A) || Input::CheckPadLStickLeft();
}

bool KochaEngine::InputManager::MoveDash()
{
    return Input::CheckKey(DIK_LSHIFT) || Input::CheckPadButton(XINPUT_GAMEPAD_X);
}
