#include "Player.h"
#include "GameObjectManager.h"
#include "Util.h"
#include "InputManager.h"

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
	isBattle = false;

	EncountReset();

	velocity.Zero();
	speed = 0.5f;

	sphere.radius = 4.0f;
	sphere.position = this->position;

	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(-10, 10, 10));
	obj->SetTexture("Resources/Texture/Character/player/player_0.png");
	obj->SetBillboardType(Object::BILLBOARD_Y);

	//パラメーターのセット(後々テキストデータから読み込めるようにする(セーブ機能))
	param.name = "Character/player.txt";
	param.texName = "player";
	param.attribute = TYPE_LIGHT;
	param.size = Vector3(10, 10, 10);
	param.level = 1;
	param.maxHP = 10;
	param.hp = 10;
	param.maxSP = 8;
	param.sp = 8;
	param.attack = 8;
	param.defence = 6;
	param.intelligence = 5;
	param.speed = 6;
	param.skillful = 4;
	param.luck = 0;
	param.exp = 0;
	param.money = 100;
}

void KochaEngine::Player::Update()
{
	gManager->CheckBlock(this, COLLISION_BLOCK);
	
	InputMove();
	MoveX();
	MoveZ();
	EncountEnemy();

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
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::Player::GetType()
{
	return FIELD_PLAYER;
}

void KochaEngine::Player::InputMove()
{
	velocity.Zero();
	speed = 0.5f;

	if (isBattle || isEncount) return;

	bool isDash = false;

	if (InputManager::MoveUp())
	{
		velocity.z = 1;
		isDash = true;
		encountCount--;
	}
	else if (InputManager::MoveDown())
	{
		velocity.z = -1;
		isDash = true;
		encountCount--;
	}
	if (InputManager::MoveLeft())
	{
		velocity.x = -1;
		isDash = true;
		encountCount--;
	}
	else if (InputManager::MoveRight())
	{
		velocity.x = 1;
		isDash = true;
		encountCount--;
	}
	velocity.normalize();

	if (isDash)
	{
		if (InputManager::MoveDash())
		{
			speed = 0.8f;
			encountCount--;
		}
	}
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

void KochaEngine::Player::EncountReset()
{
	isEncount = false;
	encountCount = Util::GetIntRand(4, 30) * 40;
}

void KochaEngine::Player::EncountEnemy()
{
	if (encountCount <= 0)
	{
		isEncount = true;
	}
}