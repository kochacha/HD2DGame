#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class SceneChangeBlock : public GameObject
	{
	private:
		int collisionType;
		float size;

	public:
		SceneChangeBlock(const Vector3& arg_position, const float arg_size);
		~SceneChangeBlock() override;

		void Initialize() override;
		void Update() override;
		GameObjectType GetType() override;
	};
}