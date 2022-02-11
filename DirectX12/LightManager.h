#pragma once
#include "PointLight.h"
#include "DirectionalLight.h"
#include "d3dx12.h"
#include <wrl.h>
#include <memory>

namespace KochaEngine
{
	using namespace Microsoft::WRL;

	class Camera;
	class LightManager
	{
	public:
		static const int DIRECTIONAL_LIGHT_NUM = 1;
		static const int POINT_LIGHT_NUM = 10;

		struct ConstBufferData
		{
			DirectX::XMMATRIX lightCamera;
			DirectX::XMMATRIX shadow;
			Vector3 eye;
			float pad1;
			Vector3 ambientColor;
			float pad2;
			DirectionalLight::ConstBufferData dirLights[DIRECTIONAL_LIGHT_NUM];
			PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
		};

		static void StaticInitialize(ID3D12Device* arg_device);
		static LightManager* Create();

		void Update();
		void Draw(ID3D12GraphicsCommandList* arg_cmdList, const UINT arg_rootParameterIndex);
		void TransferConstBuffer();

		void SetDirectionalLightIsActive(const int arg_index, const bool arg_isActive);
		void SetDirectionalLightDirection(const int arg_index, const Vector3& arg_direction);
		void SetDirectionalLightColor(const int arg_index, const Vector3& arg_lightColor);

		void SetPointLightIsActive(const int arg_index, const bool arg_isActive);
		void SetPointLightPos(const int arg_index, const Vector3& arg_lightPos);
		void SetPointLightColor(const int arg_index, const Vector3& arg_lightColor);
		void SetPointLightAtten(const int arg_index, const Vector3& arg_lightAtten);
		void SetLightCamera(std::weak_ptr<Camera> arg_lightCamera);

	private:
		std::weak_ptr<Camera> lightCamera;

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