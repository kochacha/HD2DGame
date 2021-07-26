#include "Player.h"

KochaEngine::Player::Player()
{
	obj = new Object("plane");
}

KochaEngine::Player::~Player()
{
	delete obj;
}

void KochaEngine::Player::Initialize()
{
	position.Zero();
	velocity.Zero();
	speed = 0.5f;
}

void KochaEngine::Player::Update()
{
	InputMove();
	MoveX();
	MoveZ();
	SetObjParam();
}

void KochaEngine::Player::ObjDraw(Camera* camera)
{
	if (camera == nullptr) return;
	obj->Draw(camera);
}

void KochaEngine::Player::InputMove()
{
	velocity.Zero();
	if (Input::CheckKey(DIK_W))
	{
		velocity.z = speed;
	}
	if (Input::CheckKey(DIK_S))
	{
		velocity.z = -speed;
	}
	if (Input::CheckKey(DIK_A))
	{
		velocity.x = -speed;
	}
	if (Input::CheckKey(DIK_D))
	{
		velocity.x = speed;
	}
}

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x;
}

void KochaEngine::Player::MoveZ()
{
	position.z += velocity.z;
}

void KochaEngine::Player::SetObjParam()
{
	obj->SetPosition(position);
}
