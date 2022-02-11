#pragma once
#include "GameObject.h"
#include "ActorParam.h"
#include <map>
#include <memory>

namespace KochaEngine
{
	class GameObjectManager;

	class Player : public GameObject
	{
	public:

		enum MoveState
		{
			MOVE_STOP,
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
		};

	private:
		std::weak_ptr<GameObjectManager> gManager;

		ActorParam param;
		std::map<int, std::string> skillNames;
		std::vector<int> skillMasterLevels;

		Vector3 preVelocity;

		bool isEncount;
		bool isBattle;
		bool isSkillUpdate;
		bool isNextFieldScene;

		int encountCount;
		int animationNum;
		int count;
		int animationRate;

		float speed;

		float fakeAlpha;

		void SceneChangeCheck();
		void InputMove();
		void MoveX();
		void MoveZ();
		void SetObjParam();
		void EncountEnemy();
		void SkillUpdate();
		void SetAnimationType();
		void Animation();

	public:
		Player(std::weak_ptr<GameObjectManager> arg_gManager, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void HitBlock(_Box box)override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;

		void SetIsBattle(const bool arg_isBattle) { isBattle = arg_isBattle; }
		void SetParam(const ActorParam& arg_param) { param = arg_param; isSkillUpdate = true; }
		void EncountReset();

		const ActorParam GetParam() { return param; }
		inline const bool IsEncount() { return isEncount; }
		inline const bool IsNextFieldScene() { return isNextFieldScene; }
	};
}