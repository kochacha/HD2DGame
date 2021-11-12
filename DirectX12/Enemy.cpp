#include "Enemy.h"

KochaEngine::Enemy::Enemy(const Vector3& arg_position, const ActorParam& arg_param)
{
	position = arg_position;
	param = arg_param;

	isAlpha = true;

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

void KochaEngine::Enemy::Hit()
{
}

void KochaEngine::Enemy::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::Enemy::GetType()
{
	return ENEMY;
}
