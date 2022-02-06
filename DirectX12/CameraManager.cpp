#include "CameraManager.h"
#include "Player.h"
#include "Util.h"
#include "GameSetting.h"

KochaEngine::CameraManager::CameraManager(Camera& arg_camera) : camera(arg_camera)
{
	Initialize();
}

KochaEngine::CameraManager::~CameraManager()
{
}

void KochaEngine::CameraManager::Initialize()
{
	cameraState = KochaEngine::DEFAULT_CAMERA_STATE;
	defaultEyePosition.Zero();
	battleEyePosition.Zero();
	currentEyePosition.Zero();
	defaultTargetPosition.Zero();
	battleTargetPosition.Zero();
	currentTargetPosition.Zero();
}

void KochaEngine::CameraManager::Update()
{
	switch (cameraState)
	{
	case KochaEngine::DEFAULT_CAMERA_STATE:
		DefaultCameraUpdate();

		break;
	case KochaEngine::BATTLE_CAMERA_STATE:
		BattleCameraUpdate();

		break;

	default:
		break;
	}

	camera.Update();
}

void KochaEngine::CameraManager::SetDefaultPosition()
{
	battleEyePosition = defaultEyePosition;
	battleTargetPosition = defaultTargetPosition;
}

void KochaEngine::CameraManager::DefaultCameraUpdate()
{
	//プレイヤーに追従するカメラ
	Vector3 playerPos = player->GetPosition();
	Vector3 cameraPos = Vector3(playerPos.x, playerPos.y + CAMERA_POS_Y, playerPos.z + CAMERA_POS_Z);
	Vector3 cameraTargetPos = Vector3(playerPos.x, playerPos.y + CAMERA_TARGET_POS_Y, playerPos.z + CAMERA_TARGET_POS_Z);

	currentEyePosition = cameraPos;
	defaultEyePosition = cameraPos;
	battleEyePosition = cameraPos;
	currentTargetPosition = cameraTargetPos;
	defaultTargetPosition = cameraTargetPos;
	battleTargetPosition = cameraTargetPos;

	camera.SetEye(cameraPos);
	camera.SetTarget(cameraTargetPos);
}

void KochaEngine::CameraManager::BattleCameraUpdate()
{
	const float EASING_RATE = 0.35f;
	currentEyePosition.x = Util::EaseIn(currentEyePosition.x, battleEyePosition.x, EASING_RATE);
	currentEyePosition.y = Util::EaseIn(currentEyePosition.y, battleEyePosition.y, EASING_RATE);
	currentEyePosition.z = Util::EaseIn(currentEyePosition.z, battleEyePosition.z, EASING_RATE);
	currentTargetPosition.x = Util::EaseIn(currentTargetPosition.x, battleTargetPosition.x, EASING_RATE);
	currentTargetPosition.y = Util::EaseIn(currentTargetPosition.y, battleTargetPosition.y, EASING_RATE);
	currentTargetPosition.z = Util::EaseIn(currentTargetPosition.z, battleTargetPosition.z, EASING_RATE);

	camera.SetEye(currentEyePosition);
	camera.SetTarget(currentTargetPosition);
}
