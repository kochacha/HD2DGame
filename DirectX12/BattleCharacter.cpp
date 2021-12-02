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
	cursor = new Object("plane");
	levelUpUI = new Object("plane");
	
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
	int hpDigit = 4;//Util::GetDigit(param.maxHP);
	int spDigit = 4;//Util::GetDigit(param.maxSP);
	int levelDigit = 4;//Util::GetDigit(param.level);
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
	delete cursor;
	delete levelUpUI;
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
	//‹¤’ÊÝ’è
	obj->SetPosition(position);
	obj->SetScale(param.size);
	obj->SetTexture("Resources/Texture/Character/" + param.texName + "/" + param.texName + "_0.png");

	cursor->SetScale(Vector3(2, 2, 2));
	cursor->SetTexture("Resources/Texture/UI/cursor.png");
	cursor->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));
	cursor->SetBillboardType(Object::BILLBOARD_Y);
	cursor->MoveRotate(Vector3(0, 0, 90));

	levelUpUI->SetScale(Vector3(-14, 6, 1));
	levelUpUI->SetTexture("Resources/Texture/UI/levelUp_0.png");
	levelUpUI->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));
	levelUpUI->SetBillboardType(Object::BILLBOARD_Y);
	//levelUpUI->MoveRotate(Vector3(0, 0, 90));

	switch (battleObjectType)
	{
	case KochaEngine::BATTLE_PLAYER:
		
		break;
	case KochaEngine::BATTLE_FIGHTER:

		break;
	default:

		break;
	}

	isLevelUpAnimationUpdate = false;

	knockBackTime = 0;
	getExp = 0;
	needExp = 0;
	levelUpAnimationTime = 0;
	levelUpAnimationNum = 0;

	prePosX = position.x + Util::GetIntRand(0, 3) - 60;
	activePosX = prePosX - 15;
}

void KochaEngine::BattleCharacter::Update()
{
	//if (position.x > prePosX)
	//{
	//	position.x -= 1.3f;
	//}

	EasingPosition();

	if (knockBackTime > 0)
	{
		if (knockBackTime > 10)
		{
			position.x += knockBackTime * 0.1f;
		}
		knockBackTime--;
	}

	if (levelUpAnimationTime > 0)
	{
		if (levelUpAnimationTime % 5 == 0)
		{
			isLevelUpAnimationUpdate = true;
			levelUpAnimationNum++;
		}
		levelUpAnimationTime--;
	}


	FixParam();

	CalcExp();
	SetGauge();
	SetObjParam();
}

void KochaEngine::BattleCharacter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (isTarget)
	{
		cursor->Draw(arg_camera, arg_lightManager);
	}
	if (levelUpAnimationTime != 0)
	{
		levelUpUI->Draw(arg_camera, arg_lightManager);
	}

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

void KochaEngine::BattleCharacter::AddExp(const int arg_exp)
{
	getExp = arg_exp;
}

void KochaEngine::BattleCharacter::AddMoney(const int arg_money)
{
	param.money += arg_money;
}

void KochaEngine::BattleCharacter::SetDamage(const int arg_damage)
{
	knockBackTime = 15;
	param.hp -= arg_damage;
}

void KochaEngine::BattleCharacter::EasingPosition()
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

	if (param.hp == 0 && knockBackTime <= 0)
	{
		isKnockDown = true;
	}

	if (levelUpAnimationNum > 10)
	{
		levelUpAnimationNum = 0;
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
	cursor->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));
	cursor->MoveRotate(Vector3(0, 4, 0));
	levelUpUI->SetPosition(Vector3(position.x, position.y + param.size.y / 1.5f, position.z));

	if (isLevelUpAnimationUpdate)
	{
		isLevelUpAnimationUpdate = false;
		std::string extension = std::to_string(levelUpAnimationNum) + ".png";
		levelUpUI->SetTexture("Resources/Texture/UI/levelUp_" + extension);
	}

}

void KochaEngine::BattleCharacter::CalcExp()
{
	if (getExp == 0) return;

	needExp = BASE_EXP * param.level * param.level;
	int totalExp = param.exp + getExp;

	if (totalExp >= needExp)
	{
		LevelUpStatus();
		param.level++;
		param.hp = param.maxHP;
		param.sp = param.maxSP;
		param.exp = 0;
		levelUpAnimationTime = 110;

		getExp = totalExp - needExp;
		CalcExp();
	}
	else
	{
		param.exp += getExp;
		getExp = 0;
	}

}

void KochaEngine::BattleCharacter::LevelUpStatus()
{
	switch (battleObjectType)
	{
	case KochaEngine::BATTLE_PLAYER:
		param.maxHP += 4;
		param.maxSP += 3;
		param.attack += 4;
		param.defence += 3;
		param.intelligence += 3;
		param.skillful += 2;
		param.speed += 3;
		break;
	case KochaEngine::BATTLE_FIGHTER:
		param.maxHP += 5;
		param.maxSP += 2;
		param.attack += 5;
		param.defence += 4;
		param.intelligence += 0;
		param.skillful += 4;
		param.speed += 2;
		break;
	default:
		break;
	}
}
