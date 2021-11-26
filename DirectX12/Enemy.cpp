#include "Enemy.h"
#include "BattleObjectManager.h"

KochaEngine::Enemy::Enemy(BattleObjectManager* arg_bManager, const Vector3& arg_position, const ActorParam& arg_param)
{
	if (arg_bManager == nullptr) return;
	bManager = arg_bManager;
	position = arg_position;
	param = arg_param;

	isActive = false;

	obj = new Object("plane");

	Initialize();
}

KochaEngine::Enemy::~Enemy()
{
	delete obj;
}

void KochaEngine::Enemy::Initialize()
{
	obj->SetScale(param.size);
	std::string texName = "Resources/Texture/Enemy/" + param.texName + "/" + param.texName + "_0.png"; //‰¼’u‚«
	obj->SetTexture(texName);
	obj->SetPosition(position);
	obj->SetBillboardType(Object::BILLBOARD_Y);

}

void KochaEngine::Enemy::Update()
{
}

void KochaEngine::Enemy::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Enemy::Reward()
{
	bManager->AddReward(param.money, param.exp);
}

void KochaEngine::Enemy::ActiveReset()
{
	isActive = false;
}

KochaEngine::BattleObjectType KochaEngine::Enemy::GetType()
{
	return ENEMY;
}
