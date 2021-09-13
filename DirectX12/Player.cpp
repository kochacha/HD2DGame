#include "Player.h"
#include "GameObjectManager.h"

KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	position = arg_position;

	CameraTracking();

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

	sphere.radius = 4.0f;
	sphere.position = this->position;

	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/player0.png");
}

void KochaEngine::Player::Update()
{
	gManager->CheckBlock(this, COLLISION_BLOCK);
	
	InputMove();
	MoveX();
	MoveZ();

	SetObjParam();
	CameraTracking();
}

void KochaEngine::Player::Hit()
{
}

void KochaEngine::Player::HitBlock(_Box arg_box)
{
	_Box _box = arg_box;
	CollisionFace faceX = Collision::CheckHitFaceX(sphere, _box);
	CollisionFace faceZ = Collision::CheckHitFaceZ(sphere, _box);
	XMFLOAT3 minPos = Collision::GetLeftUpFront(_box.position, _box.edgeLength);
	XMFLOAT3 maxPos = Collision::GetRightDownBack(_box.position, _box.edgeLength);

	switch (faceX)
	{
	case Face_Right:
		position.x = maxPos.x + sphere.radius;
		break;
	case Face_Left:
		position.x = minPos.x - sphere.radius;
		break;
	default:
		break;
	}
	switch (faceZ)
	{
	case Face_Back:
		position.z = maxPos.z + sphere.radius;
		break;
	case Face_Front:
		position.z = minPos.z - sphere.radius;
		break;
	default:
		break;
	}
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::Player::GetType()
{
	return PLAYER;
}

void KochaEngine::Player::InputMove()
{
	velocity.Zero();
	if (Input::CheckKey(DIK_LSHIFT))
	{
		speed = 0.8f;
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
}

void KochaEngine::Player::MoveZ()
{
	position.z += velocity.z * speed;
}

void KochaEngine::Player::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

void KochaEngine::Player::CameraTracking()
{
	Vector3 cameraPos = Vector3(position.x, position.y + 20, position.z - 60);
	Vector3 cameraTargetPos = Vector3(position.x, position.y, position.z + 20);
	camera->SetEye(cameraPos);
	camera->SetTarget(cameraTargetPos);
}
