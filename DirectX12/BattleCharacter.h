#pragma once
#include "GameObject.h"
#include "ActorParam.h"

namespace KochaEngine
{
	class BattleCharacter : public GameObject
	{
	private:
		GameObjectType gameObjectType;
		ActorParam param;

	public:
		BattleCharacter(const GameObjectType& arg_gameObjectType, const Vector3& arg_position, const ActorParam& arg_param);
		~BattleCharacter() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;

	};

}