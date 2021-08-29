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
		Player();
		~Player() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera, LightManager* arg_lightManager) override;
	};
}