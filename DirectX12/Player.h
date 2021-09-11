#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class Player : public GameObject
	{
	private:
		float speed;

		void InputMove();
		void MoveX();
		void MoveZ();
		void SetObjParam();

	public:
		Player(Camera* arg_camera, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
	};
}