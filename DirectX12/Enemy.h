#pragma once
#include "GameObject.h"
#include "ActorParam.h"

namespace KochaEngine
{
	class Enemy : public GameObject
	{
	private:
		ActorParam param;
	public:
		Enemy(const Vector3& arg_position, const ActorParam& arg_param);
		~Enemy() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;
	};
}