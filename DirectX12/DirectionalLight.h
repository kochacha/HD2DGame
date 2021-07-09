#pragma once
#include "Vector3.h"

namespace KochaEngine
{
	class DirectionalLight
	{
	private:
		Vector3 direction;

	public:
		DirectionalLight(const Vector3& arg_direction);
		~DirectionalLight();

		void SetDirection(const Vector3& arg_direction) { direction = arg_direction; }
		Vector3 GetDirection() { return direction; }
	};
}