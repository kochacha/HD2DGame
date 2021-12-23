#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class BattleObjectManager;
	class EffectManager;
	class Number3DEmitter;

	class Enemy : public BattleObject
	{
	private:
		BattleObjectManager* bManager;
		EffectManager* effectManager;
		Number3DEmitter* n3DEmitter;
		
		int knockBackTime;

		float prePosX;
		float activePosX;

		void Support(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void Damage(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void MagicDamage(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void PhysicsDamage(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void EasingPosition();
		void FixParam();
		void SetObjParam();

	public:
		Enemy(BattleObjectManager* arg_bManager, EffectManager* arg_effectManager, Number3DEmitter* arg_n3DEmitter,  const Vector3& arg_position, const ActorParam& arg_param);
		~Enemy() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;

		void SetDamage(const int arg_damage) override;
		void SetDamage(const std::string& arg_skillName, const ActorParam& arg_param) override;
		void Reward() override;
		BattleObjectType GetType() override;
	};
}