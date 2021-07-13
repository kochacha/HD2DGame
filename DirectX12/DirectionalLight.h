#pragma once
#include "Vector3.h"

namespace KochaEngine
{
	class DirectionalLight
	{
	private:
		Vector3 direction = { 1,1,1 };
		Vector3 lightColor = { 1,1,1 };
		bool isActive = false;

	public:
		struct ConstBufferData
		{
			Vector3 direction;
			float pad1;
			Vector3 lightColor;
			unsigned int isActive;
		};

		inline void SetDirection(const Vector3& arg_direction) { direction = arg_direction; }
		inline void SetLightColor(const Vector3& arg_lightColor) { direction = arg_lightColor; }
		inline void SetIsActive(bool arg_isActive) { isActive = arg_isActive; }

		inline const Vector3& GetDirection() { return direction; }
		inline const Vector3& GetLightColor() { return lightColor; }
		inline bool IsActive() { return isActive; }
	};
}