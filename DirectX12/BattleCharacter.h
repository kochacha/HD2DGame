#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class BattleCharacter : public BattleObject
	{
	private:
		BattleObjectType battleObjectType;

		float prePosX;

		void SetParam();

	public:
		BattleCharacter(const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param);
		~BattleCharacter() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		BattleObjectType GetType() override;

		void ActiveReset() override;
	};

}