#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class CollisionBlock : public GameObject
	{
	private:
		float size;

	public:
		CollisionBlock(const Vector3& arg_position, const float arg_size);
		~CollisionBlock() override;

		void Initialize() override;
		void Update() override;
		GameObjectType GetType() override;
	};
}