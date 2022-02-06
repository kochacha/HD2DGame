#include "Player.h"
#include "GameObjectManager.h"
#include "Util.h"
#include "InputManager.h"
#include "GameSetting.h"
#include "JsonLoader.h"
#include "SkillData.h"

KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	position = arg_position;

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
	isSkillUpdate = true;
	animType = AnimationType::WAIT_FLONT;

	EncountReset();

	velocity.Zero();
	preVelocity.Zero();
	speed = 0.5f;

	sphere.radius = 4.0f;
	sphere.position = this->position;

	fakeAlpha = 1.0f;

	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(-10, 10, 10));
	obj->SetColor(Vector4(1, 1, 1, fakeAlpha));
	obj->SetTexture("Resources/Texture/Character/player/player_wait_f_0.png");
	obj->SetBillboardType(Object::BILLBOARD_Y);

	//パラメーターのセット
	auto obj = JsonLoader::ReadJsonObj("Resources/DataBase/PlayerData.json");
	param.name = "Character/player.txt";
	param.texName = "player";
	param.size = Vector3(10, 10, 10);
	param.attribute = SkillData::GetAttribute(obj["Attribute"].get<double>());
	param.level = obj["Level"].get<double>();
	param.maxHP = obj["MaxHP"].get<double>();
	param.hp = obj["HP"].get<double>();
	param.maxSP = obj["MaxSP"].get<double>();
	param.sp = obj["SP"].get<double>();
	param.attack = obj["Attack"].get<double>();
	param.defence = obj["Defence"].get<double>();
	param.intelligence = obj["Intelligence"].get<double>();
	param.speed = obj["Speed"].get<double>();
	param.skillful = obj["Skillful"].get<double>();
	param.luck = obj["Luck"].get<double>();
	param.exp = obj["Exp"].get<double>();
	param.money = obj["Money"].get<double>();

	picojson::object& skills = obj["Skill"].get<picojson::object>();
	for (const auto& e : skills) {
		skillNames.emplace(skills[e.first].get<double>(), e.first);
		//skillNames.insert(std::make_pair(skills[e.first].get<double>(), e.first));
		skillMasterLevels.push_back(skills[e.first].get<double>());
	}

	count = 0;
	animationNum = 0;
	animationRate = DEFAULT_ANIMATION_RATE;
}

void KochaEngine::Player::Update()
{
	gManager->CheckBlock(this, COLLISION_BLOCK);
	
	InputMove();
	MoveX();
	MoveZ();
	EncountEnemy();

	SkillUpdate();
	Animation();
	SetObjParam();

	if (Input::TriggerKey(DIK_UP))
	{
		fakeAlpha += 0.01f;
	}
	if (Input::TriggerKey(DIK_DOWN))
	{
		fakeAlpha -= 0.01f;
	}
	obj->SetColor(Vector4(1, 1, 1, fakeAlpha));
}

void KochaEngine::Player::Hit()
{
}

void KochaEngine::Player::HitBlock(_Box arg_box)
{
	_Box _box = arg_box;
	CollisionFace faceX = Collision::CheckHitFaceX(sphere, _box);
	CollisionFace faceZ = Collision::CheckHitFaceZ(sphere, _box);
	Vector3 minPos = _box.leftUpFront;
	Vector3 maxPos = _box.rightDownBack;

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
	animationRate = DEFAULT_ANIMATION_RATE;

	if (isBattle || isEncount) return;

	bool isDash = false;

	SetAnimationType();

	if (InputManager::MoveUp())
	{
		preVelocity.Zero();
		velocity.z = 1;
		preVelocity.z = velocity.z;
		isDash = true;
		encountCount--;
		animType = AnimationType::WALK_BACK;
	}
	else if (InputManager::MoveDown())
	{
		preVelocity.Zero();
		velocity.z = -1;
		preVelocity.z = velocity.z;
		isDash = true;
		encountCount--;
		animType = AnimationType::WALK_FLONT;
	}
	if (InputManager::MoveLeft())
	{
		preVelocity.Zero();
		velocity.x = -1;
		preVelocity.x = velocity.x;
		isDash = true;
		encountCount--;
		animType = AnimationType::WALK_LEFT;
	}
	else if (InputManager::MoveRight())
	{
		preVelocity.Zero();
		velocity.x = 1;
		preVelocity.x = velocity.x;
		isDash = true;
		encountCount--;
		animType = AnimationType::WALK_RIGHT;
	}
	velocity.Normalize();

	if (isDash)
	{
		if (InputManager::MoveDash())
		{
			speed = 0.8f;
			encountCount--;
			animationRate = DASH_ANIMATION_RATE;
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

void KochaEngine::Player::EncountReset()
{
	isEncount = false;
	encountCount = Util::GetIntRand(4, 30) * 40;
}

void KochaEngine::Player::EncountEnemy()
{
	if (encountCount <= 0 && GameSetting::isBattleField)
	{
		isEncount = true;
	}
}

void KochaEngine::Player::SkillUpdate()
{
	if (!isSkillUpdate) return;
	isSkillUpdate = false;

	param.skills.clear();

	for (int i = skillMasterLevels.size() - 1; i >= 0; i--)
	{
		if (param.level >= skillMasterLevels[i])
		{
			param.skills.push_back(skillNames.at(skillMasterLevels[i]));
		}
	}
}

void KochaEngine::Player::SetAnimationType()
{
	if (preVelocity.z > 0)
	{
		animType = AnimationType::WAIT_BACK;
	}
	else if (preVelocity.z < 0)
	{
		animType = AnimationType::WAIT_FLONT;
	}
	if (preVelocity.x > 0)
	{
		animType = AnimationType::WAIT_RIGHT;
	}
	else if(preVelocity.x < 0)
	{
		animType = AnimationType::WAIT_LEFT;
	}
}

void KochaEngine::Player::Animation()
{
	count++;
	if (count % animationRate == 0)
	{
		animationNum++;
	}
	else
	{
		return;
	}
	if (animationNum > 1)
	{
		animationNum = 0;
	}

	std::string num = std::to_string(animationNum) + ".png";

	switch (animType)
	{
	case KochaEngine::WALK_FLONT:
		obj->SetTexture("Resources/Texture/Character/player/player_walk_f_" + num);
		break;
	case KochaEngine::WALK_BACK:
		obj->SetTexture("Resources/Texture/Character/player/player_walk_b_" + num);
		break;
	case KochaEngine::WALK_LEFT:
		obj->SetTexture("Resources/Texture/Character/player/player_walk_l_" + num);
		break;
	case KochaEngine::WALK_RIGHT:
		obj->SetTexture("Resources/Texture/Character/player/player_walk_r_" + num);
		break;
	case KochaEngine::WAIT_FLONT:
		obj->SetTexture("Resources/Texture/Character/player/player_wait_f_0.png");
		break;
	case KochaEngine::WAIT_BACK:
		obj->SetTexture("Resources/Texture/Character/player/player_wait_b_0.png");
		break;
	case KochaEngine::WAIT_LEFT:
		obj->SetTexture("Resources/Texture/Character/player/player_wait_l_0.png");
		break;
	case KochaEngine::WAIT_RIGHT:
		obj->SetTexture("Resources/Texture/Character/player/player_wait_r_0.png");
		break;
	default:
		break;
	}
}
