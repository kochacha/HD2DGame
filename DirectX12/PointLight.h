#pragma once
#include "Vector3.h"

namespace KochaEngine
{
	class PointLight
	{
	private:
		Vector3 lightPos = { 0,0,0 };
		Vector3 lightColor = { 1,1,1 };
		Vector3 lightAtten = { 1.0f,1.0f,1.0f };
		bool isActive = false;

	public:
		struct ConstBufferData
		{
			Vector3 lightPos;
			float pad1;
			Vector3 lightColor;
			float pad2;
			Vector3 lightAtten;
			unsigned int isActive;
		};

		inline void SetLightPos(const Vector3& arg_lightPos) { lightPos = arg_lightPos; }
		inline void SetLightColor(const Vector3& arg_lightColor) { lightColor = arg_lightColor; }
		inline void SetLightAtten(const Vector3& arg_lightAtten) { lightAtten = arg_lightAtten; }
		inline void SetIsActive(const bool arg_isActive) { isActive = arg_isActive; }

		inline const Vector3& GetLightPos() { return lightPos; }
		inline const Vector3& GetLightColor() { return lightColor; }
		inline const Vector3& GetLightAtten() { return lightAtten; }
		inline const bool IsActive() { return isActive; }
	};
}
