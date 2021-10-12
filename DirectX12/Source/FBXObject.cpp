#include "../Header/FBXObject.h"
#include "../Header/Dx12_Pipeline.h"
#include "../Header/Dx12_RootSignature.h"
#include "../Header/FBXLoader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

ID3D12Device* KochaEngine::FBXObject::device = nullptr;
KochaEngine::Camera* KochaEngine::FBXObject::camera = nullptr;

KochaEngine::FBXObject::FBXObject()
{
	Initialize();
}

KochaEngine::FBXObject::~FBXObject()
{
}

void KochaEngine::FBXObject::Initialize()
{
	HRESULT result;

	//�萔�o�b�t�@�̐���
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
	}

	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffSkin));
	}

	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void KochaEngine::FBXObject::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (model == nullptr) return;
	DirectX::XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = DirectX::XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�r���[�v���W�F�N�V�����s��
	const DirectX::XMMATRIX& matViewProjection = camera->GetMatView() * camera->GetMatProjection();
	//���f���̃��b�V���g�����X�t�H�[��
	const DirectX::XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const Vector3& cameraPos = camera->GetEye();

	HRESULT result;

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewProj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//�A�j���[�V����
	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime;
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	//�{�[���z��
	std::vector<FBXModel::Bone>& bones = model->GetBones();

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		DirectX::XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FBXLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	cmdList->SetPipelineState(Dx12_Pipeline::fbxPipelineState.Get());

	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetFBXRootSignature().Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	model->Draw(cmdList);
}

void KochaEngine::FBXObject::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}

void KochaEngine::FBXObject::SetPosition(const Vector3 arg_position)
{
	position = arg_position;
}

void KochaEngine::FBXObject::MovePosition(const Vector3 move)
{
	this->position.x += move.x;
	this->position.y += move.y;
	this->position.z += move.z;
}

void KochaEngine::FBXObject::MoveScale(const Vector3 moveScale)
{
	this->scale.x += moveScale.x;
	this->scale.y += moveScale.y;
	this->scale.z += moveScale.z;
}

void KochaEngine::FBXObject::MoveRotate(const Vector3 moveRotate)
{
	this->rotation.x += moveRotate.x;
	this->rotation.y += moveRotate.y;
	this->rotation.z += moveRotate.z;
}