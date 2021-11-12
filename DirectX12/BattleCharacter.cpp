#include "BattleCharacter.h"

KochaEngine::BattleCharacter::BattleCharacter(const GameObjectType& arg_gameObjectType, const Vector3& arg_position, const ActorParam& arg_param)
{
	gameObjectType = arg_gameObjectType;
	position = arg_position;
	param = arg_param;

	isAlpha = true;

	obj = new Object("plane");

	Initialize();
}

KochaEngine::BattleCharacter::~BattleCharacter()
{
	delete obj;
}

void KochaEngine::BattleCharacter::Initialize()
{
	//obj->SetTexture();
}

void KochaEngine::BattleCharacter::Update()
{
}

void KochaEngine::BattleCharacter::Hit()
{
}

void KochaEngine::BattleCharacter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::BattleCharacter::GetType()
{
	return gameObjectType;
}
