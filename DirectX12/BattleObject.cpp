#include "BattleObject.h"

KochaEngine::BattleObject::BattleObject()
{
}

KochaEngine::BattleObject::~BattleObject()
{
}

void KochaEngine::BattleObject::Initialize()
{
}

void KochaEngine::BattleObject::Update()
{
}

void KochaEngine::BattleObject::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
}

void KochaEngine::BattleObject::SpriteDraw()
{
}

void KochaEngine::BattleObject::Dead()
{
}

void KochaEngine::BattleObject::Reward()
{
}

void KochaEngine::BattleObject::ActiveReset()
{
	isActive = false;
}

void KochaEngine::BattleObject::ActiveDone()
{
	isActive = true;
}

void KochaEngine::BattleObject::CurrentActive()
{
	isCurrentActive = true;
}

void KochaEngine::BattleObject::CurrentActiveReset()
{
	isCurrentActive = false;
}

void KochaEngine::BattleObject::TargetOn()
{
	isTarget = true;
}

void KochaEngine::BattleObject::TargetOff()
{
	isTarget = false;
}

void KochaEngine::BattleObject::AddExp(const int arg_exp)
{
}

void KochaEngine::BattleObject::AddMoney(const int arg_money)
{
}

void KochaEngine::BattleObject::SetDamage(const int arg_damage)
{
}

KochaEngine::BattleObjectType KochaEngine::BattleObject::GetType()
{
	return BattleObjectType();
}
