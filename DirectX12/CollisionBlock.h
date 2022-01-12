#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	enum
	{
		COLLISION_DEFAULT,
		COLLISION_UP,
		COLLISION_DOWN,
		COLLISION_RIGHT,
		COLLISION_LEFT,
	};

	class CollisionBlock : public GameObject
	{
	private:
		int collisionType;
		float size;

	public:
		CollisionBlock(const Vector3& arg_position, const float arg_size, const int arg_collisionType = 0);
		~CollisionBlock() override;

		void Initialize() override;
		void Update() override;
		GameObjectType GetType() override;
	};
}