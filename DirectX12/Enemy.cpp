#include "Enemy.h"
#include "BattleObjectManager.h"

KochaEngine::Enemy::Enemy(BattleObjectManager* arg_bManager, const Vector3& arg_position, const ActorParam& arg_param)
{
	if (arg_bManager == nullptr) return;
	bManager = arg_bManager;
	position = arg_position;
	param = arg_param;

	obj = new Object("plane");

	Initialize();
}

KochaEngine::Enemy::~Enemy()
{
	delete obj;
}

void KochaEngine::Enemy::Initialize()
{
	isActive = false;
	isKnockDown = false;
	obj->SetScale(param.size);
	std::string texName = "Resources/Texture/Enemy/" + param.texName + "/" + param.texName + "_0.png"; //‰¼’u‚«
	obj->SetTexture(texName);
	obj->SetPosition(position);
	obj->SetBillboardType(Object::BILLBOARD_Y);

}

void KochaEngine::Enemy::Update()
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

	if (param.hp == 0)
	{
		isKnockDown = true;
	}
}

void KochaEngine::Enemy::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Enemy::SetDamage(const int arg_damage)
{
	param.hp -= arg_damage;
}

void KochaEngine::Enemy::Reward()
{
	bManager->AddReward(param.money, param.exp);
}

void KochaEngine::Enemy::ActiveReset()
{
	isActive = false;
}

void KochaEngine::Enemy::ActiveDone()
{
	isActive = true;
}

KochaEngine::BattleObjectType KochaEngine::Enemy::GetType()
{
	return ENEMY;
}
