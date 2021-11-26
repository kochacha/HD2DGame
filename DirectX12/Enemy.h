#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class BattleObjectManager;

	class Enemy : public BattleObject
	{
	private:
		BattleObjectManager* bManager;

	public:
		Enemy(BattleObjectManager* arg_bManager, const Vector3& arg_position, const ActorParam& arg_param);
		~Enemy() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void Reward() override;
		void ActiveReset() override;
		BattleObjectType GetType() override;
	};
}