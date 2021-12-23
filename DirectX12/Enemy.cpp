#include "Enemy.h"
#include "BattleObjectManager.h"
#include "EffectManager.h"
#include "Util.h"
#include "Number3DEmitter.h"
#include "SkillData.h"

KochaEngine::Enemy::Enemy(BattleObjectManager* arg_bManager, EffectManager* arg_effectManager, Number3DEmitter* arg_n3DEmitter, const Vector3& arg_position, const ActorParam& arg_param)
{
	if (!arg_bManager) return;
	if (!arg_effectManager) return;
	if (!arg_n3DEmitter) return;

	bManager = arg_bManager;
	effectManager = arg_effectManager;
	n3DEmitter = arg_n3DEmitter;
	position = arg_position;
	baseParam = arg_param;
	battleParam = arg_param;

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

	obj->SetScale(Vector3(-baseParam.size.x, baseParam.size.y, baseParam.size.z));
	std::string texName = "Resources/Texture/Enemy/" + baseParam.texName + "/" + baseParam.texName + "_0.png"; //仮置き
	obj->SetTexture(texName);
	obj->SetPosition(position);
	obj->SetBillboardType(Object::BILLBOARD_Y);

	cursor->SetScale(Vector3(2, 2, 2));
	cursor->SetTexture("Resources/Texture/UI/cursor.png");
	cursor->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));
	cursor->SetBillboardType(Object::BILLBOARD_Y);
	cursor->MoveRotate(Vector3(0, 0, 90));

	knockBackTime = 0;

	prePosX = position.x;
	activePosX = prePosX + 20;
}

void KochaEngine::Enemy::Update()
{
	EasingPosition();

	if (knockBackTime > 0)
	{
		if (knockBackTime > 15)
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
	knockBackTime = 20;
	baseParam.hp -= arg_damage;
}

void KochaEngine::Enemy::SetDamage(const std::string& arg_skillName, const ActorParam& arg_param)
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

void KochaEngine::Enemy::Support(const SkillParam& arg_activeData, const ActorParam& arg_param)
{

}

void KochaEngine::Enemy::Damage(const SkillParam& arg_activeData, const ActorParam& arg_param)
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

void KochaEngine::Enemy::MagicDamage(const SkillParam& arg_activeData, const ActorParam& arg_param)
{
	//ダメージ計算処理
	auto activeParam = arg_param;

	//基礎ダメージ
	int damage = activeParam.intelligence * arg_activeData.intelligenceRate + arg_activeData.baseDamage;
	//運ダメージ範囲
	int luckDamageRange = damage * ((float)activeParam.luck * arg_activeData.luckRate * 0.002f) + 2;
	//運ダメージ範囲がマイナスだった場合の補正
	if (luckDamageRange < 0) luckDamageRange = 0;
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
	knockBackTime = 20;
}

void KochaEngine::Enemy::PhysicsDamage(const SkillParam& arg_activeData, const ActorParam& arg_param)
{
	//ダメージ計算処理
	auto activeParam = arg_param;

	//基礎ダメージ
	int damage = activeParam.attack * arg_activeData.attackRate - battleParam.defence * 0.25f + arg_activeData.baseDamage;
	//運ダメージ範囲
	int luckDamageRange = damage * ((float)activeParam.luck * arg_activeData.luckRate * 0.002f) + 2;
	//運ダメージ範囲がマイナスだった場合の補正
	if (luckDamageRange < 0) luckDamageRange = 0;
	//運ダメージ
	int luckDamage = Util::GetIntRand(0, luckDamageRange) - luckDamageRange * 0.25f;
	//トータルダメージ
	int totalDamage = damage + luckDamage;

	//基礎ダメージを下回っていた場合補正する
	if (totalDamage < arg_activeData.baseDamage) totalDamage = arg_activeData.baseDamage;

	//ダメージor回復を受ける
	baseParam.hp -= totalDamage;

	//ダメージ表記を出す位置
	Vector3 addDamagePos = Vector3(position.x, position.y + 2, position.z - 0.01f);

	//ダメージ表記
	n3DEmitter->AddNumber3D(addDamagePos, totalDamage);

	//エフェクトの再生
	effectManager->Play(arg_activeData.effectName, position);

	//ノックバックの追加
	knockBackTime = 20;
}

void KochaEngine::Enemy::Reward()
{
	bManager->AddReward(baseParam.money, baseParam.exp);
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

	if (baseParam.hp == 0 && knockBackTime <= 0 && !isKnockDown)
	{
		isKnockDown = true;
		effectManager->Play("DeadEnemy.efk", position);
	}
}

void KochaEngine::Enemy::SetObjParam()
{
	obj->SetPosition(position);
	cursor->SetPosition(Vector3(position.x, position.y + baseParam.size.y / 1.5f, position.z));
	cursor->MoveRotate(Vector3(0, 4, 0));
}


KochaEngine::BattleObjectType KochaEngine::Enemy::GetType()
{
	return ENEMY;
}
