#include "Camera.h"
#include<iostream>
#include"Input.h"
#include"Quaternion.h"
#include "Util.h"

using namespace DirectX;

KochaEngine::Camera::Camera()
{
}

KochaEngine::Camera::~Camera()
{
}

void KochaEngine::Camera::Initialize(float WIN_WIDTH, float WIN_HEIGHT, float angle, float distance, Vector3 eye, Vector3 target, Vector3 up)
{
	cameraHAngle = 0;
	cameraVAngle = 0;
	cameraDistance = distance;
	cameraSpeed = 2;
	crrentAngle = 0;

	shakeTime = 0;
	shakePower = 0;
	nowTime = 0;

	this->eye = eye;
	this->target = target;
	this->up = up;

	preEye = eye;
	preTarget = target;

	shakeFlag = false;
	H = 0;
	V = 0;

	matProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f),
		WIN_WIDTH / WIN_HEIGHT,
		0.1f, 1500.0f);

	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
}

void KochaEngine::Camera::Update()
{	
	
	if (Input::CheckKey(DIK_LEFT))
	{
		cameraHAngle += 1;	
	}
	if (Input::CheckKey(DIK_RIGHT))
	{
		cameraHAngle -= 1;	
	}
	if (Input::CheckKey(DIK_UP))
	{
		cameraVAngle += 1;	
	}
	if (Input::CheckKey(DIK_DOWN))
	{
		cameraVAngle -= 1;
	}
	//if (cameraVAngle <= 40 && cameraVAngle >= -23)
	//{
	//	cameraVAngle -= Input::GetRStickDirection().y * cameraSpeed / 2;
	//}

	crrentAngle -= Input::GetRStickDirection().x * cameraSpeed;
	if (shakeFlag)
	{
		H -= Input::GetRStickDirection().x;
		V -= Input::GetRStickDirection().y;
	}
	//cameraHAngle = Util::Lerp(cameraHAngle, crrentAngle, 0.5f);
	
	
	

	Vector3 TempPosition1;
	Vector3 TempPosition2;
	Vector3 CameraLookAtPosition;

	// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
	CameraLookAtPosition = target;
	

	// カメラの位置はカメラの水平角度と垂直角度から算出

	// 最初に垂直角度を反映した位置を算出
	float SinParam = sin(cameraVAngle / 180.0f * 3.14159265f);
	float CosParam = cos(cameraVAngle / 180.0f * 3.14159265f);
	TempPosition1.x = 0.0f;
	TempPosition1.y = SinParam * cameraDistance;
	TempPosition1.z = -CosParam * cameraDistance;

	// 次に水平角度を反映した位置を算出
	SinParam = sin(cameraHAngle / 180.0f * 3.14159265f);
	CosParam = cos(cameraHAngle / 180.0f * 3.14159265f);
	TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// 算出した座標に注視点の位置を加算したものがカメラの位置
	eye.x = TempPosition2.x + CameraLookAtPosition.x;
	eye.y = TempPosition2.y + CameraLookAtPosition.y;
	eye.z = TempPosition2.z + CameraLookAtPosition.z;

	//Shake(shakeTime, shakePower);

	// カメラの設定に反映する
	ViewUpdate();
}

void KochaEngine::Camera::ViewUpdate()
{
	//ビュー変換行列
	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
}

void KochaEngine::Camera::SetEye(Vector3 eye)
{
	this->eye = eye;
	ViewUpdate();
}

void KochaEngine::Camera::SetTarget(Vector3 target)
{
	this->target = target;
	ViewUpdate();
}

void KochaEngine::Camera::SetUp(Vector3 up)
{
	this->up = up;
	ViewUpdate();
}

void KochaEngine::Camera::SetAngle(float angle)
{
	crrentAngle = angle;
}

void KochaEngine::Camera::MoveEye(Vector3 vel)
{
	this->eye.x += vel.x;
	this->eye.y += vel.y;
	this->eye.z += vel.z;
	ViewUpdate();
}

float KochaEngine::Camera::Getangle()
{
	return cameraHAngle;
}

float KochaEngine::Camera::GetCameraSpeed()
{
	float result;
	result = cameraSpeed * 2;
	return result;
}

void KochaEngine::Camera::CameraSpeedUp()
{
	if (cameraSpeed < 4.5f)
	{
		this->cameraSpeed += 0.5f;
	}
	
}

void KochaEngine::Camera::CameraSpeedDown()
{
	if (cameraSpeed > 0.5f)
	{
		this->cameraSpeed -= 0.5f;
	}
}

void KochaEngine::Camera::Shake(float time, float power)
{

	//// 経過時間が継続時間以下なら
	//if (nowTime < time)
	//{
	//	// X方向とY方向にランダムな速度を得る
	//	float x;
	//	float y;
	//	if (nowTime % 2)
	//	{
	//		x = -power;
	//		y = -power;
	//	}
	//	else
	//	{
	//		x = power;
	//		y = power;
	//	}
	//	

	//	// カメラの座標にセット
	//	cameraDistance += x;
	//	//cameraVAngle += y;
	//	// カウントアップ
	//	nowTime++;
	//}
	//else if (nowTime >= time)
	//{
	//	// 始点座標、注視点座標を元の位置に戻す

	//	shakeTime = 0;
	//	shakePower = 0;
	//	nowTime = 0;
	//	cameraDistance = 80;
	//}
}

void KochaEngine::Camera::SetShake(float time, float power)
{
	shakeTime = time;
	shakePower = power;
	shakeFlag = true;
}


void KochaEngine::Camera::UpCamera()
{
	
}

void KochaEngine::Camera::RightCamera()
{
	
}

KochaEngine::Vector3 KochaEngine::Camera::GetVelocity()
{
	Vector3 result;
	result.x = target.x - eye.x;
	result.y = target.y - eye.y;
	result.z = target.z - eye.z;
	XMVECTOR vec = XMLoadFloat3(&result);
	vec = XMVector3Normalize(vec);
	XMStoreFloat3(&result, vec);
	if (result.y <= 0) result.y = 0;
	return result;
}


