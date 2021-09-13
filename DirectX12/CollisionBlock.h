#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class CollisionBlock : public GameObject
	{
	private:


	public:
		CollisionBlock(const Vector3& arg_position);
		~CollisionBlock() override;

		void Initialize() override;
		void Update() override;
		GameObjectType GetType() override;
	};
}