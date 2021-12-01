#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class BattleObjectManager;

	class Enemy : public BattleObject
	{
	private:
		BattleObjectManager* bManager;
		
		int knockBackTime;

		float prePosX;
		float activePosX;

		void EasingPosition();
		void FixParam();
		void SetObjParam();

	public:
		Enemy(BattleObjectManager* arg_bManager, const Vector3& arg_position, const ActorParam& arg_param);
		~Enemy() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;

		void SetDamage(const int arg_damage) override;
		void Reward() override;
		BattleObjectType GetType() override;
	};
}