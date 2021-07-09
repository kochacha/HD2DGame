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
	
	//if (Input::CheckKey(DIK_LEFT))
	//{
	//	cameraHAngle += 1;	
	//}
	//if (Input::CheckKey(DIK_RIGHT))
	//{
	//	cameraHAngle -= 1;	
	//}
	//if (Input::CheckKey(DIK_UP))
	//{
	//	cameraVAngle += 1;	
	//}
	//if (Input::CheckKey(DIK_DOWN))
	//{
	//	cameraVAngle -= 1;
	//}
	////if (cameraVAngle <= 40 && cameraVAngle >= -23)
	////{
	////	cameraVAngle -= Input::GetRStickDirection().y * cameraSpeed / 2;
	////}

	//crrentAngle -= Input::GetRStickDirection().x * cameraSpeed;
	//if (shakeFlag)
	//{
	//	H -= Input::GetRStickDirection().x;
	//	V -= Input::GetRStickDirection().y;
	//}
	////cameraHAngle = Util::Lerp(cameraHAngle, crrentAngle, 0.5f);
	
	
	

	//Vector3 TempPosition1;
	//Vector3 TempPosition2;
	//Vector3 CameraLookAtPosition;

	//// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
	//CameraLookAtPosition = target;
	//

	//// カメラの位置はカメラの水平角度と垂直角度から算出

	//// 最初に垂直角度を反映した位置を算出
	//float SinParam = sin(cameraVAngle / 180.0f * 3.14159265f);
	//float CosParam = cos(cameraVAngle / 180.0f * 3.14159265f);
	//TempPosition1.x = 0.0f;
	//TempPosition1.y = SinParam * cameraDistance;
	//TempPosition1.z = -CosParam * cameraDistance;

	//// 次に水平角度を反映した位置を算出
	//SinParam = sin(cameraHAngle / 180.0f * 3.14159265f);
	//CosParam = cos(cameraHAngle / 180.0f * 3.14159265f);
	//TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	//TempPosition2.y = TempPosition1.y;
	//TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	//// 算出した座標に注視点の位置を加算したものがカメラの位置
	//eye.x = TempPosition2.x + CameraLookAtPosition.x;
	//eye.y = TempPosition2.y + CameraLookAtPosition.y;
	//eye.z = TempPosition2.z + CameraLookAtPosition.z;

	//Shake(shakeTime, shakePower);

	Billboard();

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

void KochaEngine::Camera::Billboard()
{
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

	// 全方向ビルボード行列の計算
	{
		matBillboard.r[0] = cameraAxisX;
		matBillboard.r[1] = cameraAxisY;
		matBillboard.r[2] = cameraAxisZ;
		matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	}

	//Y軸回りビルボード行列の計算
	{
		// カメラX軸、Y軸、Z軸
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		// X軸は共通
		ybillCameraAxisX = cameraAxisX;
		// Y軸はワールド座標系のY軸
		ybillCameraAxisY = XMVector3Normalize(upVector);
		// Z軸はX軸→Y軸の外積で求まる
		ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

		// Y軸回りビルボード行列
		matBillboardY.r[0] = ybillCameraAxisX;
		matBillboardY.r[1] = ybillCameraAxisY;
		matBillboardY.r[2] = ybillCameraAxisZ;
		matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
	}

}

void KochaEngine::Camera::DebugCamera()
{

}

void KochaEngine::Camera::SetEye(const Vector3& eye)
{
	this->eye = eye;
	ViewUpdate();
}

void KochaEngine::Camera::SetTarget(const Vector3& target)
{
	this->target = target;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::SetUp(const Vector3& up)
{
	this->up = up;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::SetAngle(float angle)
{
	crrentAngle = angle;
}

void KochaEngine::Camera::MoveEye(const Vector3& vel)
{
	this->eye.x += vel.x;
	this->eye.y += vel.y;
	this->eye.z += vel.z;
	this->target.x += vel.x;
	this->target.y += vel.y;
	this->target.z += vel.z;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::MoveTarget(const Vector3& vel)
{
	this->target.x += vel.x;
	this->target.y += vel.y;
	this->target.z += vel.z;
	Billboard();
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


