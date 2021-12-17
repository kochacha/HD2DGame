#include "BattleCharacter.h"
#include "Util.h"
#include "Texture2D.h"
#include "Text.h"
#include "GameSetting.h"
#include "Number.h"
#include "EffectManager.h"
#include "Number3DEmitter.h"

KochaEngine::BattleCharacter::BattleCharacter(EffectManager* arg_effectManager, Number3DEmitter* arg_n3DEmitter, const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param)
{
	effectManager = arg_effectManager;
	n3DEmitter = arg_n3DEmitter;
	battleObjectType = arg_battleObjectType;
	position = arg_position;
	baseParam = arg_param;

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
	nameText = new Text(baseParam.name, charaNamePos, Vector2(32, 32));

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
	//共通設定
	obj->SetPosition(position);
	obj->SetScale(Vector3(-baseParam.size.x, baseParam.size.y, baseParam.size.z));
	obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_wait_0.png");

	cursor->SetScale(Vector3(2, 2, 2));
	cursor->SetTexture("Resources/Texture/UI/cursor.png");
	cursor->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));
	cursor->SetBillboardType(Object::BILLBOARD_Y);
	cursor->MoveRotate(Vector3(0, 0, 90));

	levelUpUI->SetScale(Vector3(-10, 4, 1));
	levelUpUI->SetTexture("Resources/Texture/UI/levelUp_0.png");
	levelUpUI->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));
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

	startPosX = position.x;
	prePosX = startPosX + Util::GetIntRand(0, 3) - 57;
	activePosX = prePosX - 22;
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
		if (knockBackTime > 20)
		{
			position.x += knockBackTime * 0.05f;
		}
		knockBackTime--;
	}
	if (knockBackTime == 1)
	{
		obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_wait_0.png");
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
	paramTex[0]->Draw(baseParam.maxHP);
	paramTex[1]->Draw(baseParam.hp);
	paramTex[2]->Draw(baseParam.maxSP);
	paramTex[3]->Draw(baseParam.sp);
	paramTex[4]->Draw(baseParam.level);
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
	baseParam.money += arg_money;
}

void KochaEngine::BattleCharacter::SetDamage(const int arg_damage)
{
	baseParam.hp -= arg_damage;
	knockBackTime = 25;
	obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_attack_0.png");
}

void KochaEngine::BattleCharacter::SetDamage(const std::string& arg_skillName, const ActorParam& arg_param)
{
	//スキルデータ
	SkillParam activeData = SkillData::GetSkillParam(arg_skillName);

	if (activeData.isSupport)
	{
		Support(activeData, arg_param);
	}
	else
	{
		Damage(activeData, arg_param);
	}
}

void KochaEngine::BattleCharacter::SetDefaultWaitTexture()
{
	obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_wait_0.png");
}

void KochaEngine::BattleCharacter::SetAttackTextureIndex(const int arg_index)
{
	if (arg_index == 0)
	{
		obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_attack_0.png");
	}
	else
	{
		obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_attack_1.png");
	}
}

void KochaEngine::BattleCharacter::Support(const SkillParam& arg_activeData, const ActorParam& arg_param)
{

}

void KochaEngine::BattleCharacter::Damage(const SkillParam& arg_activeData, const ActorParam& arg_param)
{
	if (arg_activeData.isMagic)
	{
		MagicDamage(arg_activeData, arg_param);
	}
	else
	{
		PhysicsDamage(arg_activeData, arg_param);
	}
}

void KochaEngine::BattleCharacter::MagicDamage(const SkillParam& arg_activeData, const ActorParam& arg_param)
{
}

void KochaEngine::BattleCharacter::PhysicsDamage(const SkillParam& arg_activeData, const ActorParam& arg_param)
{
	//ダメージ計算処理
	auto activeParam = arg_param;

	//基礎ダメージ
	int damage = activeParam.attack * arg_activeData.attackRate - baseParam.defence * 0.25f;
	//運ダメージ範囲
	int luckDamageRange = damage * ((float)activeParam.luck * arg_activeData.luckRate * 0.002f) + 2;
	//運ダメージ
	int luckDamage = Util::GetIntRand(0, luckDamageRange) - luckDamageRange * 0.25f;
	//トータルダメージ
	int totalDamage = damage + luckDamage;

	//基礎ダメージを下回っていた場合補正する
	if (totalDamage < arg_activeData.baseDamage) totalDamage = arg_activeData.baseDamage;

	//ダメージを受ける
	baseParam.hp -= totalDamage;

	//ダメージ表記を出す位置
	Vector3 addDamagePos = Vector3(position.x, position.y + 2, position.z - 0.01f);

	//ダメージ表記
	n3DEmitter->AddNumber3D(addDamagePos, totalDamage);

	//エフェクトの再生
	effectManager->Play(arg_activeData.effectName, position);

	//ノックバックの追加
	knockBackTime = 25;

	//ダメージを食らっているテクスチャに変える
	obj->SetTexture("Resources/Texture/Character/" + baseParam.texName + "/" + baseParam.texName + "_attack_0.png");
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
	if (baseParam.hp > baseParam.maxHP)
	{
		baseParam.hp = baseParam.maxHP;
	}
	else if (baseParam.hp < 0)
	{
		baseParam.hp = 0;
	}
	if (baseParam.sp > baseParam.maxSP)
	{
		baseParam.sp = baseParam.maxSP;
	}
	else if (baseParam.sp < 0)
	{
		baseParam.sp = 0;
	}

	if (baseParam.hp == 0 && knockBackTime <= 0)
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
	float hpRate = (float)baseParam.hp / (float)baseParam.maxHP;
	const float HP_BAR_SIZE_X = MAX_GAUGE_SIZE.x * hpRate;
	float spRate = (float)baseParam.sp / (float)baseParam.maxSP;
	const float SP_BAR_SIZE_X = MAX_GAUGE_SIZE.x * spRate;

	gaugeTex[1]->SetSize(Vector2(HP_BAR_SIZE_X, MAX_GAUGE_SIZE.y));
	gaugeTex[3]->SetSize(Vector2(SP_BAR_SIZE_X, MAX_GAUGE_SIZE.y));
}

void KochaEngine::BattleCharacter::SetObjParam()
{
	obj->SetPosition(position);
	cursor->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));
	cursor->MoveRotate(Vector3(0, 4, 0));
	levelUpUI->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));

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

	needExp = BASE_EXP * baseParam.level * baseParam.level;
	int totalExp = baseParam.exp + getExp;

	if (totalExp >= needExp)
	{
		LevelUpStatus();
		baseParam.level++;
		baseParam.hp = baseParam.maxHP;
		baseParam.sp = baseParam.maxSP;
		baseParam.exp = 0;
		levelUpAnimationTime = 110;

		getExp = totalExp - needExp;
		CalcExp();
		effectManager->Play("levelUp.efk", Vector3(position.x, position.y - 4.0f, position.z));
	}
	else
	{
		baseParam.exp += getExp;
		getExp = 0;
	}

}

void KochaEngine::BattleCharacter::LevelUpStatus()
{
	switch (battleObjectType)
	{
	case KochaEngine::BATTLE_PLAYER:
		baseParam.maxHP += 4;
		baseParam.maxSP += 3;
		baseParam.attack += 4;
		baseParam.defence += 3;
		baseParam.intelligence += 3;
		baseParam.skillful += 2;
		baseParam.speed += 3;
		break;
	case KochaEngine::BATTLE_FIGHTER:
		baseParam.maxHP += 5;
		baseParam.maxSP += 2;
		baseParam.attack += 5;
		baseParam.defence += 4;
		baseParam.intelligence += 0;
		baseParam.skillful += 4;
		baseParam.speed += 2;
		break;
	default:
		break;
	}
}
