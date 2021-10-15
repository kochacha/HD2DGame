#include "Object.h"
#include "Dx12_Texture.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Descriptor.h"
#include "Dx12_Pipeline.h"
#include "LightManager.h"
#include <codecvt>

#pragma comment(lib, "d3dcompiler.lib")

//KochaEngine::LightManager* KochaEngine::Object::lightManager{};
ID3D12Device* KochaEngine::Object::device{};
ID3D12GraphicsCommandList* KochaEngine::Object::cmdList{};
SIZE KochaEngine::Object::winSize{};

KochaEngine::Object::Object(std::string objName) : objName(objName)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring ws = converter.from_bytes(objName);
	std::string modelname(ws.begin(), ws.end());
	std::string filename = KochaEngine::Dx12_Object::GetMaterial(objName).objFilename;
	std::string directoryPath = "Resources/Object/" + modelname + "/";
	texName = directoryPath + modelname + ".png";

	//�t�@�C���p�X������
	std::string filepath = directoryPath + filename;

	CreateBufferView();
	//CreateDepthStencilView();
}

KochaEngine::Object::~Object()
{
}

//void KochaEngine::Object::SetLightManager(LightManager* arg_lightManager)
//{
//	if (arg_lightManager == nullptr) return;
//	Object::lightManager = arg_lightManager;
//}

void KochaEngine::Object::StaticInit(ID3D12Device* device, SIZE winSize)
{
	KochaEngine::Object::winSize = winSize;

	if (device == nullptr) return;
	KochaEngine::Object::device = device;
}

void KochaEngine::Object::BeginDraw(ID3D12GraphicsCommandList* arg_cmdList)
{
	if (arg_cmdList == nullptr) return;
	KochaEngine::Object::cmdList = arg_cmdList;

	arg_cmdList->SetPipelineState(Dx12_Pipeline::objPipelineState.Get());
	arg_cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetOBJRootSignature().Get());
	arg_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void KochaEngine::Object::BeginAlphaDraw(ID3D12GraphicsCommandList* arg_cmdList)
{
	if (arg_cmdList == nullptr) return;
	KochaEngine::Object::cmdList = arg_cmdList;

	arg_cmdList->SetPipelineState(Dx12_Pipeline::alphaObjPipelineState.Get());
	arg_cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetOBJRootSignature().Get());
	arg_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void KochaEngine::Object::BeginDrawFromLight(ID3D12GraphicsCommandList* arg_cmdList)
{
	if (arg_cmdList == nullptr) return;
	KochaEngine::Object::cmdList = arg_cmdList;

	arg_cmdList->SetPipelineState(Dx12_Pipeline::shadowPipelineState.Get());
	arg_cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetOBJRootSignature().Get());
	arg_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void KochaEngine::Object::EndDraw()
{
	KochaEngine::Object::cmdList = nullptr;
}

void KochaEngine::Object::CreateBufferView()
{
	basicHeapHandle = Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart();

	HRESULT result;
	// �萔�o�b�t�@�̐���
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp, 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB0));
	}
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp, 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1));
	}

	basicHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize);

	srvDesc.Format = Dx12_Texture::GetTexResDesc(texName).Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		Dx12_Texture::GetTexBuff(texName).Get(),
		&srvDesc,
		cpuDescHandleSRV);
}

void KochaEngine::Object::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = winSize.cx;
	depthResDesc.Height = winSize.cy;
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	
	auto result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		Dx12_Descriptor::GetDepthHeap().Get()->GetCPUDescriptorHandleForHeapStart());
}

void KochaEngine::Object::Draw(Camera* camera, LightManager* arg_lightManager)
{
	if (camera == nullptr)	return;
	if (arg_lightManager == nullptr) return;

	HRESULT result;
	DirectX::XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate.z));
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotate.x));
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotate.y));
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s��̍���
	matWorld = DirectX::XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	switch (billboardType)
	{
	case BillboardType::NONE:
		break;
	case BillboardType::BILLBOARD:
		matWorld *= camera->GetBillboardMatrix(); //�r���{�[�h�𔽉f
		break;
	case BillboardType::BILLBOARD_Y:
		matWorld *= camera->GetBillboardYMatrix(); //Y���r���{�[�h�𔽉f
		break;
	default:
		break;
	}


	matView = camera->GetMatView();
	matProjection = camera->GetMatProjection();

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->color = color;
	constMap0->mat = matWorld * matView * matProjection;
	constMap0->world = matWorld;
	constMap0->cameraPos = camera->GetEye();
	constBuffB0->Unmap(0, nullptr);

	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	auto material = KochaEngine::Dx12_Object::GetMaterial(objName);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);

	auto vbView = KochaEngine::Dx12_Object::GetVBView(objName);
	auto ibView = KochaEngine::Dx12_Object::GetIBView(objName);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	arg_lightManager->Draw(cmdList, 3);
	cmdList->SetGraphicsRootDescriptorTable(4, gpuDescHandleSRV);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)KochaEngine::Dx12_Object::GetIndices(objName).size(), 1, 0, 0, 0);
}

void KochaEngine::Object::Draw(Camera* camera, LightManager* arg_lightManager, Vector3 position, Vector3 scale, Vector3 rotate)
{
	if (camera == nullptr)	return;
	if (arg_lightManager == nullptr) return;

	HRESULT result;
	DirectX::XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate.z));
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotate.x));
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotate.y));
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s��̍���
	matWorld = DirectX::XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	switch (billboardType)
	{
	case BillboardType::NONE:
		break;
	case BillboardType::BILLBOARD:
		matWorld *= camera->GetBillboardMatrix(); //�r���{�[�h�𔽉f
		break;
	case BillboardType::BILLBOARD_Y:
		matWorld *= camera->GetBillboardYMatrix(); //Y���r���{�[�h�𔽉f
		break;
	default:
		break;
	}

	matView = camera->GetMatView();
	matProjection = camera->GetMatProjection();

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->color = color;
	constMap0->mat = matWorld * matView * matProjection;
	constMap0->world = matWorld;
	constMap0->cameraPos = camera->GetEye();
	constBuffB0->Unmap(0, nullptr);

	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	auto material = KochaEngine::Dx12_Object::GetMaterial(objName);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);


	auto vbView = KochaEngine::Dx12_Object::GetVBView(objName);
	auto ibView = KochaEngine::Dx12_Object::GetIBView(objName);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);

	arg_lightManager->Draw(cmdList, 3);

	cmdList->SetGraphicsRootDescriptorTable(4, gpuDescHandleSRV);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)KochaEngine::Dx12_Object::GetIndices(objName).size(), 1, 0, 0, 0);
}

void KochaEngine::Object::SetPosition(const Vector3& position)
{
	this->position = position;
}

void KochaEngine::Object::SetScale(const Vector3& scale)
{
	this->scale = scale;
}

void KochaEngine::Object::SetRotate(const Vector3& rotate)
{
	this->rotate = rotate;
}

void KochaEngine::Object::SetColor(const Vector4& color)
{
	this->color = color;
}

void KochaEngine::Object::SetAlpha(const float arg_alpha)
{
	this->color.w = arg_alpha;
}

void KochaEngine::Object::SetTexture(const std::string& textureName)
{
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(textureName), descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(textureName), descriptorHandleIncrementSize);

	srvDesc.Format = Dx12_Texture::GetTexResDesc(textureName).Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		Dx12_Texture::GetTexBuff(textureName).Get(),
		&srvDesc,
		cpuDescHandleSRV);
}

void KochaEngine::Object::SetBillboardType(const BillboardType& arg_type)
{
	billboardType = arg_type;
}

void KochaEngine::Object::MovePosition(const Vector3& move)
{
	this->position.x += move.x;
	this->position.y += move.y;
	this->position.z += move.z;
}

void KochaEngine::Object::MoveScale(const Vector3& moveScale)
{
	this->scale.x += moveScale.x;
	this->scale.y += moveScale.y;
	this->scale.z += moveScale.z;
}

void KochaEngine::Object::MoveRotate(const Vector3& moveRotate)
{
	this->rotate.x += moveRotate.x;
	this->rotate.y += moveRotate.y;
	this->rotate.z += moveRotate.z;
}

void KochaEngine::Object::MoveColor(const Vector4& moveColor)
{
	this->color.x += moveColor.x;
	this->color.y += moveColor.y;
	this->color.z += moveColor.z;
	this->color.w += moveColor.w;
}
