#include "Player.h"

KochaEngine::Player::Player(Camera* arg_camera, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	camera = arg_camera;
	position = arg_position;

	Vector3 cameraPos = Vector3(position.x, position.y + 20, position.z - 60);
	Vector3 cameraTargetPos = Vector3(position.x, position.y, position.z + 20);
	camera->SetEye(cameraPos);
	camera->SetTarget(cameraTargetPos);

	obj = new Object("plane");
	Initialize();
}

KochaEngine::Player::~Player()
{
	delete obj;
}

void KochaEngine::Player::Initialize()
{
	isAlpha = true;

	velocity.Zero();
	speed = 0.5f;

	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/PIEN.png");
}

void KochaEngine::Player::Update()
{
	InputMove();
	MoveX();
	MoveZ();
	SetObjParam();
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Player::InputMove()
{
	velocity.Zero();
	if (Input::CheckKey(DIK_LSHIFT))
	{
		speed = 1.0f;
	}
	else
	{
		speed = 0.5f;
	}
	if (Input::CheckKey(DIK_W))
	{
		velocity.z = 1;
	}
	if (Input::CheckKey(DIK_S))
	{
		velocity.z = -1;
	}
	if (Input::CheckKey(DIK_A))
	{
		velocity.x = -1;
	}
	if (Input::CheckKey(DIK_D))
	{
		velocity.x = 1;
	}
	velocity.normalize();
}

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x * speed;
	camera->MoveEye(Vector3(velocity.x * speed, 0, 0));
}

void KochaEngine::Player::MoveZ()
{
	position.z += velocity.z * speed;
	camera->MoveEye(Vector3(0, 0, velocity.z * speed));
}

void KochaEngine::Player::SetObjParam()
{
	obj->SetPosition(position);
}
