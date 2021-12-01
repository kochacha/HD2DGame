#pragma once
#include "GameObject.h"
#include "ActorParam.h"

namespace KochaEngine
{
	class Fighter : public GameObject
	{
	private:
		ActorParam param;

	public:
		Fighter(Camera* arg_camera, const Vector3& arg_position);
		~Fighter() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;

		void SetParam(const ActorParam& arg_param) { param = arg_param; }

		const ActorParam GetParam() { return param; }
	};
}