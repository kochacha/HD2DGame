#include "Enemy.h"
#include "BattleObjectManager.h"
#include "Util.h"

KochaEngine::Enemy::Enemy(BattleObjectManager* arg_bManager, const Vector3& arg_position, const ActorParam& arg_param)
{
	if (arg_bManager == nullptr) return;
	bManager = arg_bManager;
	position = arg_position;
	param = arg_param;

	obj = new Object("plane");
	cursor = new Object("plane");

	Initialize();
}

KochaEngine::Enemy::~Enemy()
{
	delete obj;
	delete cursor;
}

void KochaEngine::Enemy::Initialize()
{
	isActive = false;
	isKnockDown = false;

	obj->SetScale(Vector3(-param.size.x, param.size.y, param.size.z));
	std::string texName = "Resources/Texture/Enemy/" + param.texName + "/" + param.texName + "_0.png"; //‰¼’u‚«
	obj->SetTexture(texName);
	obj->SetPosition(position);
	obj->SetBillboardType(Object::BILLBOARD_Y);

	cursor->SetScale(Vector3(2, 2, 2));
	cursor->SetTexture("Resources/Texture/UI/cursor.png");
	cursor->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));
	cursor->SetBillboardType(Object::BILLBOARD_Y);
	cursor->MoveRotate(Vector3(0, 0, 90));

	knockBackTime = 0;

	prePosX = position.x;
	activePosX = prePosX + 25;
}

void KochaEngine::Enemy::Update()
{
	EasingPosition();

	if (knockBackTime > 0)
	{
		if (knockBackTime > 10)
		{
			position.x -= knockBackTime * 0.1f;
		}
		knockBackTime--;
	}

	FixParam();

	SetObjParam();
}

void KochaEngine::Enemy::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (isKnockDown) return;
	if (isTarget)
	{
		cursor->Draw(arg_camera, arg_lightManager);
	}
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Enemy::SetDamage(const int arg_damage)
{
	knockBackTime = 15;
	param.hp -= arg_damage;
}

void KochaEngine::Enemy::Reward()
{
	bManager->AddReward(param.money, param.exp);
}

void KochaEngine::Enemy::EasingPosition()
{
	if (knockBackTime != 0) return;

	const float EASING_RATE = 0.4f;
	if (isCurrentActive)
	{
		position.x = Util::EaseIn(position.x, activePosX, EASING_RATE);
	}
	else
	{
		position.x = Util::EaseIn(position.x, prePosX, EASING_RATE);
	}
}

void KochaEngine::Enemy::FixParam()
{
	if (param.hp > param.maxHP)
	{
		param.hp = param.maxHP;
	}
	else if (param.hp < 0)
	{
		param.hp = 0;
	}
	if (param.sp > param.maxSP)
	{
		param.sp = param.maxSP;
	}
	else if (param.sp < 0)
	{
		param.sp = 0;
	}

	if (param.hp == 0 && knockBackTime <= 0)
	{
		isKnockDown = true;
	}
}

void KochaEngine::Enemy::SetObjParam()
{
	obj->SetPosition(position);
	cursor->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));
	cursor->MoveRotate(Vector3(0, 4, 0));
}


KochaEngine::BattleObjectType KochaEngine::Enemy::GetType()
{
	return ENEMY;
}
