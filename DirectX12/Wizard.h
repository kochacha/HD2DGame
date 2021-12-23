#pragma once
#include "GameObject.h"
#include "ActorParam.h"

namespace KochaEngine
{
	class Wizard : public GameObject
	{
	private:
		ActorParam param;

	public:
		Wizard(Camera* arg_camera, const Vector3& arg_position);
		~Wizard() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;

		void SetParam(const ActorParam& arg_param) { param = arg_param; }

		const ActorParam GetParam() { return param; }
	};
}