#pragma once
#include "GameObject.h"
#include "ActorParam.h"
#include <map>

namespace KochaEngine
{
	class Fighter : public GameObject
	{
	private:
		ActorParam param;
		std::map<int, std::string> skillNames;
		std::vector<int> skillMasterLevels;

		bool isSkillUpdate;

		void SkillUpdate();

	public:
		Fighter(const Vector3& arg_position);
		~Fighter() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType() override;

		void SetParam(const ActorParam& arg_param) { param = arg_param; isSkillUpdate = true; }

		const ActorParam GetParam() { return param; }
	};
}