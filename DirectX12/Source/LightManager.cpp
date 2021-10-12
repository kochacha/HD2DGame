#include "../Header/LightManager.h"
#include "../Header/Camera.h"

ID3D12Device* KochaEngine::LightManager::device = nullptr;

void KochaEngine::LightManager::StaticInitialize(ID3D12Device* arg_device)
{
	assert(!LightManager::device);
	assert(arg_device);
	LightManager::device = arg_device;
}

KochaEngine::LightManager* KochaEngine::LightManager::Create()
{
	LightManager* instance = new LightManager();
	instance->Initialize();
	return instance;
}

void KochaEngine::LightManager::DefaultLightSetting()
{
	dirLights[0].SetIsActive(true);
	dirLights[0].SetLightColor(Vector3(1, 1, 1));
	dirLights[0].SetDirection(Vector3(1, 1, 1));

	for (int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		pointLights[i].SetIsActive(false);
		pointLights[i].SetLightColor(Vector3(1, 1, 1));
		pointLights[i].SetLightPos(Vector3(0, 0, 0));
	}
	pointLights[0].SetIsActive(true);
	pointLights[0].SetLightAtten(Vector3(1, 1, 1));
}

void KochaEngine::LightManager::Initialize()
{
	assert(device);

	DefaultLightSetting();

	HRESULT result;
	// �萔�o�b�t�@�̐���
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);// �A�b�v���[�h�\
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff));
		if (FAILED(result)) assert(0);
	}

	// �萔�o�b�t�@�փf�[�^�]��
	//TransferConstBuffer();
}

void KochaEngine::LightManager::Update()
{
	if (isDirty) 
	{
		TransferConstBuffer();
		isDirty = false;
	}
}

void KochaEngine::LightManager::Draw(ID3D12GraphicsCommandList* arg_cmdList, UINT arg_rootParameterIndex)
{
	if (arg_cmdList == nullptr) return;
	arg_cmdList->SetGraphicsRootConstantBufferView(arg_rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void KochaEngine::LightManager::TransferConstBuffer()
{
	HRESULT result;
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		//constMap->lightCamera = lightCamera->GetLightCameraMatrix();
		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DIRECTIONAL_LIGHT_NUM; i++)
		{
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].isActive = true;
				constMap->dirLights[i].direction = dirLights[i].GetDirection();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].isActive = false;
			}
		}

		for (int i = 0; i < POINT_LIGHT_NUM; i++)
		{
			if (pointLights[i].IsActive())
			{
				constMap->pointLights[i].isActive = true;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			}
			else
			{
				constMap->pointLights[i].isActive = false;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void KochaEngine::LightManager::SetDirectionalLightIsActive(int arg_index, bool arg_isActive)
{
	assert(0 <= arg_index && arg_index < DIRECTIONAL_LIGHT_NUM);
	dirLights[arg_index].SetIsActive(arg_isActive);
}

void KochaEngine::LightManager::SetDirectionalLightDirection(int arg_index, const Vector3& arg_direction)
{
	assert(0 <= arg_index && arg_index < DIRECTIONAL_LIGHT_NUM);
	dirLights[arg_index].SetDirection(arg_direction);
	isDirty = true;
}

void KochaEngine::LightManager::SetDirectionalLightColor(int arg_index, const Vector3& arg_lightColor)
{
	assert(0 <= arg_index && arg_index < DIRECTIONAL_LIGHT_NUM);
	dirLights[arg_index].SetLightColor(arg_lightColor);
	isDirty = true;
}

void KochaEngine::LightManager::SetPointLightIsActive(int arg_index, bool arg_isActive)
{
	assert(0 <= arg_index && arg_index < POINT_LIGHT_NUM);
	pointLights[arg_index].SetIsActive(arg_isActive);
}

void KochaEngine::LightManager::SetPointLightPos(int arg_index, const Vector3& arg_lightPos)
{
	assert(0 <= arg_index && arg_index < POINT_LIGHT_NUM);
	pointLights[arg_index].SetLightPos(arg_lightPos);
	isDirty = true;
}

void KochaEngine::LightManager::SetPointLightColor(int arg_index, const Vector3& arg_lightColor)
{
	assert(0 <= arg_index && arg_index < POINT_LIGHT_NUM);
	pointLights[arg_index].SetLightColor(arg_lightColor);
	isDirty = true;
}

void KochaEngine::LightManager::SetPointLightAtten(int arg_index, const Vector3& arg_lightAtten)
{
	assert(0 <= arg_index && arg_index < POINT_LIGHT_NUM);
	pointLights[arg_index].SetLightAtten(arg_lightAtten);
	isDirty = true;
}

void KochaEngine::LightManager::SetLightCamera(Camera* arg_lightCamera)
{
	if (arg_lightCamera == nullptr)return;
	lightCamera = arg_lightCamera;
}
