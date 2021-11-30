#include "BattleCharacter.h"
#include "Util.h"
#include "Texture2D.h"
#include "Text.h"
#include "GameSetting.h"
#include "Number.h"

KochaEngine::BattleCharacter::BattleCharacter(const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param)
{
	battleObjectType = arg_battleObjectType;
	position = arg_position;
	param = arg_param;

	isActive = false;

	obj = new Object("plane");
	
	Vector2 charaStatusPos;
	Vector2 charaNamePos;

	switch (battleObjectType)
	{
	case KochaEngine::BATTLE_PLAYER:
		charaStatusPos = BATTLE_STATUS_POS;
		charaNamePos = BATTLE_STATUS_POS + Vector2(5, 5);
		break;
	case KochaEngine::BATTLE_FIGHTER:
		charaStatusPos = BATTLE_STATUS_POS - Vector2(0, 200);
		charaNamePos = BATTLE_STATUS_POS + Vector2(5, 5) - Vector2(0, 200);
		break;
	default:
		break;
	}
	battleStatusTex = new Texture2D("Resources/Texture/UI/command_2.png", charaStatusPos, BATTLE_STATUS_SIZE, 0);
	nameText = new Text(param.name, charaNamePos, Vector2(32, 32));

	Vector2 hpPos = charaStatusPos + Vector2(10, 110);
	Vector2 spPos = charaStatusPos + Vector2(10, 158);
	gaugeTex[0] = new Texture2D("Resources/Texture/Color/darkRed.png", hpPos, MAX_GAUGE_SIZE, 0);
	gaugeTex[1] = new Texture2D("Resources/Texture/Color/green.png", hpPos, MAX_GAUGE_SIZE, 0);
	gaugeTex[2] = new Texture2D("Resources/Texture/Color/darkRed.png", spPos, MAX_GAUGE_SIZE, 0);
	gaugeTex[3] = new Texture2D("Resources/Texture/Color/lightBlue.png", spPos, MAX_GAUGE_SIZE, 0);

	Vector2 numSize = Vector2(16, 16);
	int hpDigit = Util::GetDigit(param.maxHP);
	int spDigit = Util::GetDigit(param.maxSP);
	int levelDigit = Util::GetDigit(param.level);
	paramTex[0] = new Number(hpPos + Vector2(155, -25), numSize, hpDigit);
	paramTex[1] = new Number(hpPos + Vector2(55, -25), numSize, hpDigit);
	paramTex[2] = new Number(spPos + Vector2(155, -25), numSize, spDigit);
	paramTex[3] = new Number(spPos + Vector2(55, -25), numSize, spDigit);
	paramTex[4] = new Number(hpPos + Vector2(55, -57), numSize, levelDigit);

	slashTex[0] = new Texture2D("Resources/Texture/UI/slash.png", hpPos + Vector2(133, -25), numSize, 0);
	slashTex[1] = new Texture2D("Resources/Texture/UI/slash.png", spPos + Vector2(133, -25), numSize, 0);

	Initialize();
}

KochaEngine::BattleCharacter::~BattleCharacter()
{
	delete obj;
	delete battleStatusTex;
	delete nameText;
	for (int i = 0; i < 5; i++)
	{
		if (i < 2)
		{
			delete slashTex[i];
		}
		if (i < 4)
		{
			delete gaugeTex[i];
		}
		delete paramTex[i];
	}
}

void KochaEngine::BattleCharacter::Initialize()
{
	//���ʐݒ�
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

	prePosX = position.x - 60 + (Util::GetIntRand(0, 15) - 5);
}

void KochaEngine::BattleCharacter::Update()
{
	if (position.x > prePosX)
	{
		position.x -= 1.2f;
	}

	FixParam();

	if (param.hp == 0)
	{
		isKnockDown = true;
	}

	SetGauge();
	SetObjParam();
}

void KochaEngine::BattleCharacter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::BattleCharacter::SpriteDraw()
{
	battleStatusTex->Draw();
	nameText->Draw(0);
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{
			slashTex[i]->Draw();
		}
		gaugeTex[i]->Draw();
	}
	paramTex[0]->Draw(param.maxHP);
	paramTex[1]->Draw(param.hp);
	paramTex[2]->Draw(param.maxSP);
	paramTex[3]->Draw(param.sp);
	paramTex[4]->Draw(param.level);
}

KochaEngine::BattleObjectType KochaEngine::BattleCharacter::GetType()
{
	return battleObjectType;
}

void KochaEngine::BattleCharacter::SetDamage(const int arg_damage)
{
	param.hp -= arg_damage;
}

void KochaEngine::BattleCharacter::ActiveReset()
{
	isActive = false;
}

void KochaEngine::BattleCharacter::ActiveDone()
{
	isActive = true;
}

void KochaEngine::BattleCharacter::FixParam()
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
}

void KochaEngine::BattleCharacter::SetGauge()
{
	float hpRate = (float)param.hp / (float)param.maxHP;
	const float HP_BAR_SIZE_X = MAX_GAUGE_SIZE.x * hpRate;
	float spRate = (float)param.sp / (float)param.maxSP;
	const float SP_BAR_SIZE_X = MAX_GAUGE_SIZE.x * spRate;

	gaugeTex[1]->SetSize(Vector2(HP_BAR_SIZE_X, MAX_GAUGE_SIZE.y));
	gaugeTex[3]->SetSize(Vector2(SP_BAR_SIZE_X, MAX_GAUGE_SIZE.y));
}

void KochaEngine::BattleCharacter::SetObjParam()
{
	obj->SetPosition(position);
}