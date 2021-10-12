#include "../Header/Camera.h"
#include "../Header/Input.h"
#include "../Header/Quaternion.h"
#include "../Header/Util.h"
#include<iostream>

using namespace DirectX;

KochaEngine::Camera::Camera()
{
}

KochaEngine::Camera::~Camera()
{
}

void KochaEngine::Camera::Initialize(float WIN_WIDTH, float WIN_HEIGHT, float angle, float distance, Vector3 eye, Vector3 target, Vector3 up)
{
	winSize = Vector2(WIN_WIDTH, WIN_HEIGHT);
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

	nearZ = 0.1f;
	farZ = 1500.0f;

	matProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f),
		WIN_WIDTH / WIN_HEIGHT,
		nearZ, farZ);

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

	//// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
	//CameraLookAtPosition = target;
	//

	//// �J�����̈ʒu�̓J�����̐����p�x�Ɛ����p�x����Z�o

	//// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
	//float SinParam = sin(cameraVAngle / 180.0f * 3.14159265f);
	//float CosParam = cos(cameraVAngle / 180.0f * 3.14159265f);
	//TempPosition1.x = 0.0f;
	//TempPosition1.y = SinParam * cameraDistance;
	//TempPosition1.z = -CosParam * cameraDistance;

	//// ���ɐ����p�x�𔽉f�����ʒu���Z�o
	//SinParam = sin(cameraHAngle / 180.0f * 3.14159265f);
	//CosParam = cos(cameraHAngle / 180.0f * 3.14159265f);
	//TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	//TempPosition2.y = TempPosition1.y;
	//TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	//// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
	//eye.x = TempPosition2.x + CameraLookAtPosition.x;
	//eye.y = TempPosition2.y + CameraLookAtPosition.y;
	//eye.z = TempPosition2.z + CameraLookAtPosition.z;

	//Shake(shakeTime, shakePower);

	Billboard();

	// �J�����̐ݒ�ɔ��f����
	ViewUpdate();
}

void KochaEngine::Camera::ViewUpdate()
{
	//�r���[�ϊ��s��
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

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// �J������Y���i������j
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s����v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;

	// �S�����r���{�[�h�s��̌v�Z
	{
		matBillboard.r[0] = cameraAxisX;
		matBillboard.r[1] = cameraAxisY;
		matBillboard.r[2] = cameraAxisZ;
		matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	}

	//Y�����r���{�[�h�s��̌v�Z
	{
		XMVECTOR yBillCameraAxisX, yBillCameraAxisY, yBillCameraAxisZ;

		yBillCameraAxisX = cameraAxisX;
		yBillCameraAxisY = XMVector3Normalize(upVector);
		yBillCameraAxisZ = XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);

		matBillboardY.r[0] = yBillCameraAxisX;
		matBillboardY.r[1] = yBillCameraAxisY;
		matBillboardY.r[2] = yBillCameraAxisZ;
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

XMMATRIX KochaEngine::Camera::GetLightCameraMatrix()
{
	return matView * DirectX::XMMatrixOrthographicLH(200, 200, nearZ, farZ);
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

	//// �o�ߎ��Ԃ��p�����Ԉȉ��Ȃ�
	//if (nowTime < time)
	//{
	//	// X������Y�����Ƀ����_���ȑ��x�𓾂�
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

	//	// �J�����̍��W�ɃZ�b�g
	//	cameraDistance += x;
	//	//cameraVAngle += y;
	//	// �J�E���g�A�b�v
	//	nowTime++;
	//}
	//else if (nowTime >= time)
	//{
	//	// �n�_���W�A�����_���W�����̈ʒu�ɖ߂�

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


