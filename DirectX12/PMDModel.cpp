#include "PMDModel.h"
#include "Dx12_Texture.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Descriptor.h"
#include "Dx12_Pipeline.h"
#include "PMDLoader.h"
#include <codecvt>

#pragma comment(lib, "d3dcompiler.lib")

ID3D12Device* KochaEngine::PMDModel::device{};
ID3D12GraphicsCommandList* KochaEngine::PMDModel::cmdList{};
SIZE KochaEngine::PMDModel::winSize{};

KochaEngine::PMDModel::PMDModel(std::string pmdName) : pmdName(pmdName)
{
	CreateBufferView();
}

KochaEngine::PMDModel::~PMDModel()
{
}

void KochaEngine::PMDModel::StaticInit(ID3D12Device* device, SIZE winSize)
{
	KochaEngine::PMDModel::winSize = winSize;

	if (device == nullptr) return;
	KochaEngine::PMDModel::device = device;
}

void KochaEngine::PMDModel::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	if (cmdList == nullptr) return;
	KochaEngine::PMDModel::cmdList = cmdList;

	cmdList->SetPipelineState(Dx12_Pipeline::pmdPipelineState.Get());
	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetPMDRootSignature().Get());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void KochaEngine::PMDModel::EndDraw()
{
	KochaEngine::PMDModel::cmdList = nullptr;
}

void KochaEngine::PMDModel::CreateBufferView()
{
	basicHeapHandle = Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart();

	HRESULT result;
	// 定数バッファの生成
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp, 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff));
	}

	basicHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart(), 0,descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), 0,descriptorHandleIncrementSize);

	auto pmdMaterials = PMDLoader::GetPMDMesh(pmdName)->pmdMaterials;
	materials.resize(pmdMaterials.size());
	//コピー
	for (int i = 0; i < pmdMaterials.size(); ++i)
	{
		materials[i].indicesNum = pmdMaterials[i].indicesNum;
		materials[i].material.diffuse = pmdMaterials[i].diffuse;
		materials[i].material.alpha = pmdMaterials[i].alpha;
		materials[i].material.specular = pmdMaterials[i].specular;
		materials[i].material.specularity = pmdMaterials[i].specularity;
		materials[i].material.ambient = pmdMaterials[i].ambient;
	}

	auto materialBuffSize = sizeof(MaterialForHlsl);
	materialBuffSize = (materialBuffSize + 0xff) & ~0xff;
	//マテリアルバッファーを作成
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(materialBuffSize * pmdMaterials.size());
		result = device->CreateCommittedResource(
			&heapProp, 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&materialBuff));
	}

	//マップマテリアルにコピー
	char* mapMaterial = nullptr;

	result = materialBuff->Map(0, nullptr, (void**)&mapMaterial);
	for (auto& m : materials)
	{
		*((MaterialForHlsl*)mapMaterial) = m.material; //データコピー
		mapMaterial += materialBuffSize; //次のアライメント位置まで進める
	}
	materialBuff->Unmap(0, nullptr);

	//マテリアル用ディスクリプタヒープ
	matDescHespDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	matDescHespDesc.NodeMask = 0;
	matDescHespDesc.NumDescriptors = pmdMaterials.size(); //マテリアル数
	matDescHespDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = device->CreateDescriptorHeap(&matDescHespDesc, IID_PPV_ARGS(&materialDescHeap));

	matCBVDesc.BufferLocation = materialBuff->GetGPUVirtualAddress(); //バッファーアドレス
	matCBVDesc.SizeInBytes = materialBuffSize; //マテリアルの256アライメントサイズ

	//先頭を記録
	auto matDescHeapH = materialDescHeap.Get()->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < pmdMaterials.size(); ++i)
	{
		device->CreateConstantBufferView(&matCBVDesc, matDescHeapH);
		matDescHeapH.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		matCBVDesc.BufferLocation += materialBuffSize;
	}

}

void KochaEngine::PMDModel::Draw(Camera* camera)
{
	if (camera == nullptr)	return;

	// スケール、回転、平行移動行列の計算
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate.z));
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotate.x));
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotate.y));
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	matWorld = DirectX::XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	matView = camera->GetMatView();
	matProjection = camera->GetMatProjection();

	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	auto pmdMesh = PMDLoader::GetPMDMesh(pmdName);
	auto vbView = pmdMesh->vbView;
	auto ibView = pmdMesh->ibView;
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	ID3D12DescriptorHeap* mtHeaps[] = { materialDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	cmdList->SetDescriptorHeaps(1, mtHeaps);

	auto materialH = materialDescHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	unsigned int indexOffset = 0; //最初はオフセットなし
	for (auto& m : materials)
	{
		cmdList->SetGraphicsRootDescriptorTable(1, materialH);
		cmdList->DrawIndexedInstanced(m.indicesNum, 1, indexOffset, 0, 0);
		materialH.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		indexOffset += m.indicesNum;
	}
	
	// 描画コマンド
	//cmdList->DrawIndexedInstanced((UINT)PMDLoader::GetIndices(pmdName).size(), 1, 0, 0, 0);
}

void KochaEngine::PMDModel::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
}

void KochaEngine::PMDModel::SetScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
}

void KochaEngine::PMDModel::SetRotate(DirectX::XMFLOAT3 rotate)
{
	this->rotate = rotate;
}

void KochaEngine::PMDModel::SetColor(DirectX::XMFLOAT4 color)
{
	this->color = color;
}

void KochaEngine::PMDModel::SetTexture(std::string textureName)
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

void KochaEngine::PMDModel::MovePosition(DirectX::XMFLOAT3 move)
{
	this->position.x += move.x;
	this->position.y += move.y;
	this->position.z += move.z;
}

void KochaEngine::PMDModel::MoveScale(DirectX::XMFLOAT3 moveScale)
{
	this->scale.x += moveScale.x;
	this->scale.y += moveScale.y;
	this->scale.z += moveScale.z;
}

void KochaEngine::PMDModel::MoveRotate(DirectX::XMFLOAT3 moveRotate)
{
	this->rotate.x += moveRotate.x;
	this->rotate.y += moveRotate.y;
	this->rotate.z += moveRotate.z;
}

void KochaEngine::PMDModel::MoveColor(DirectX::XMFLOAT4 moveColor)
{
	this->color.x += moveColor.x;
	this->color.y += moveColor.y;
	this->color.z += moveColor.z;
	this->color.w += moveColor.w;
}
