#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class SceneSpawnBlock : public GameObject
	{
	private:
		int collisionType;
		float size;

	public:
		SceneSpawnBlock(const SceneChangePoint& arg_sceneChangePoint, const Vector3& arg_position, const float arg_size);
		~SceneSpawnBlock() override;

		void Initialize() override;
		void Update() override;
		GameObjectType GetType() override;
	};
}