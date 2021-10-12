#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;

		bool isEncount;

		int encountCount;

		float speed;

		void InputMove();
		void MoveX();
		void MoveZ();
		void SetObjParam();
		void CameraTracking();
		void EncountEnemy();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void HitBlock(_Box box)override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();

		bool IsEncount() { return isEncount; }
	};
}