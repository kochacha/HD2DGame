#pragma once
#include "PointLight.h"
#include "DirectionalLight.h"
#include "d3dx12.h"
#include <wrl.h>

namespace KochaEngine
{
	using namespace Microsoft::WRL;

	class LightManager
	{
	public:
		static const int DIRECTIONAL_LIGHT_NUM = 1;
		static const int POINT_LIGHT_NUM = 3;

		struct ConstBufferData
		{
			Vector3 ambientColor;
			float pad1;
			DirectionalLight::ConstBufferData dirLights[DIRECTIONAL_LIGHT_NUM];
			PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
		};

		static void StaticInitialize(ID3D12Device* arg_device);
		static LightManager* Create();

		void Update();
		void Draw(ID3D12GraphicsCommandList* arg_cmdList, UINT arg_rootParameterIndex);
		void TransferConstBuffer();

		void SetDirectionalLightIsActive(int arg_index, bool arg_isActive);
		void SetDirectionalLightDirection(int arg_index, const Vector3& arg_direction);
		void SetDirectionalLightColor(int arg_index, const Vector3& arg_lightColor);

		void SetPointLightIsActive(int arg_index, bool arg_isActive);
		void SetPointLightPos(int arg_index, const Vector3& arg_lightPos);
		void SetPointLightColor(int arg_index, const Vector3& arg_lightColor);
		void SetPointLightAtten(int arg_index, const Vector3& arg_lightAtten);

	private:
		ComPtr<ID3D12Resource> constBuff;
		Vector3 ambientColor = { 1,1,1 };
		bool isDirty = false;

		DirectionalLight dirLights[DIRECTIONAL_LIGHT_NUM];
		PointLight pointLights[POINT_LIGHT_NUM];

		void DefaultLightSetting();
		void Initialize();

		static ID3D12Device* device;
	};
}