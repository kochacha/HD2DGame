#include "BattleCharacter.h"
#include "Util.h"

KochaEngine::BattleCharacter::BattleCharacter(const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param)
{
	battleObjectType = arg_battleObjectType;
	position = arg_position;
	param = arg_param;

	isActive = false;

	obj = new Object("plane");

	Initialize();
}

KochaEngine::BattleCharacter::~BattleCharacter()
{
	delete obj;
}

void KochaEngine::BattleCharacter::Initialize()
{
	//‹¤’ÊÝ’è
	obj->SetPosition(position);
	obj->SetScale(param.size);
	obj->SetTexture("Resources/Texture/Character/" + param.texName + "/" + param.texName + "_0.png");

	switch (battleObjectType)
	{
	case KochaEngine::BATTLE_PLAYER:
		
		break;
	case KochaEngine::BATTLE_FIGHTER:

		break;
	default:

		break;
	}

	prePosX = position.x - 60 + (Util::GetIntRand(0,15) - 5);
}

void KochaEngine::BattleCharacter::Update()
{
	if (position.x > prePosX)
	{
		position.x -= 1.2f;
	}

	SetParam();
}

void KochaEngine::BattleCharacter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::BattleObjectType KochaEngine::BattleCharacter::GetType()
{
	return battleObjectType;
}

void KochaEngine::BattleCharacter::ActiveReset()
{
	isActive = false;
}

void KochaEngine::BattleCharacter::SetParam()
{
	obj->SetPosition(position);
}