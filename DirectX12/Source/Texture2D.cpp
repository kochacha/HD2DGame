#include "../Header/Texture2D.h"
#include "../Header/Dx12_Texture.h"
#include "../Header/Dx12_Wrapper.h"
#include "../Header/Dx12_Blob.h"
#include "../Header/Dx12_RootSignature.h"
#include "../Header/Dx12_Descriptor.h"
#include "../Header/Dx12_Pipeline.h"

ID3D12Device* KochaEngine::Texture2D::device{};
ID3D12GraphicsCommandList* KochaEngine::Texture2D::cmdList{};
SIZE KochaEngine::Texture2D::winSize{};
UINT KochaEngine::Texture2D::descriptorHandleIncrementSize{};

KochaEngine::Texture2D::Texture2D(const std::string& texName, Vector2 position, Vector2 size, float rotate)
	: texName(texName), position(position), size(size), rotate(rotate)
{
	SetVertices();
	CreateDepthStencilView();
	CreateBufferView();
}

KochaEngine::Texture2D::Texture2D(const std::string& texName, UINT x, UINT y, UINT texNum, Vector2 position, Vector2 size, float rotate)
	: texName(texName), cutX(x), cutY(y), texNum(texNum), position(position), size(size), rotate(rotate)
{
	maxTex = cutX * cutY;
	cutSizeX = 1.0f / (float)cutX;
	cutSizeY = 1.0f / (float)cutY;
	SetCutVertices(texNum);
	CreateDepthStencilView();
	CreateBufferView();
}

KochaEngine::Texture2D::~Texture2D()
{
}

void KochaEngine::Texture2D::StaticInit(ID3D12Device* device, SIZE winSize)
{
	KochaEngine::Texture2D::winSize = winSize;

	if (device == nullptr) return;
	KochaEngine::Texture2D::device = device;

	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void KochaEngine::Texture2D::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	if (cmdList == nullptr) return;
	KochaEngine::Texture2D::cmdList = cmdList;

	cmdList->SetPipelineState(Dx12_Pipeline::spritePipelineState.Get());
	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetSpriteRootSignature().Get());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void KochaEngine::Texture2D::EndDraw()
{
	KochaEngine::Texture2D::cmdList = nullptr;
}

void KochaEngine::Texture2D::SetVertices()
{
	vertices[0] = { {0,size.y,0},{0.0f,1.0f} };
	vertices[1] = { {0,0,0},{0.0f,0.0f} };
	vertices[2] = { {size.x,size.y,0},{1.0f,1.0f} };
	vertices[3] = { {size.x,0,0},{1.0f,0.0f} };
}

void KochaEngine::Texture2D::SetSize(Vector2 size)
{
	vertices[0] = { {0,size.y,0},{0.0f,1.0f} };
	vertices[1] = { {0,0,0},{0.0f,0.0f} };
	vertices[2] = { {size.x,size.y,0},{1.0f,1.0f} };
	vertices[3] = { {size.x,0,0},{1.0f,0.0f} };
	auto result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	vertBuff->Unmap(0, nullptr);

}

void KochaEngine::Texture2D::SetCutVertices(UINT texNum)
{
	int a = 0;
	int b, c;

	for (UINT y = 0; y < cutY; y++)
	{
		for (UINT x = 0; x < cutX; x++)
		{
			a++;
			if (a == texNum)
			{
				b = x;
				c = y;
			}
		}
	}

	vertices[0] = { {0,size.y,0},{(float)b * cutSizeX,(float)c * cutSizeY + cutSizeY} };
	vertices[1] = { {0,0,0},{(float)b * cutSizeX,(float)c * cutSizeY} };
	vertices[2] = { {size.x,size.y,0},{(float)b * cutSizeX + cutSizeX,(float)c * cutSizeY + cutSizeY} };
	vertices[3] = { {size.x,0,0},{(float)b * cutSizeX + cutSizeX,(float)c * cutSizeY} };
}

void KochaEngine::Texture2D::CreateDepthStencilView()
{
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	vertBuff->Unmap(0, nullptr);

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void KochaEngine::Texture2D::CreateBufferView()
{
	basicHeapHandle = Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart();

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(ConstBufferData) + 0xff)&~0xff;
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	result = constBuff->Map(0, nullptr, (void**)&constMap);

	constMap->color = this->color;

	matRot = DirectX::XMMatrixIdentity();
	matWorld = DirectX::XMMatrixIdentity();
	constMap->mat = DirectX::XMMatrixIdentity();

	matWorld *= DirectX::XMMatrixTranslation(position.x, position.y, 0);

	matProjection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, (float)winSize.cx,
		(float)winSize.cy, 0.0f,
		0.0f, 1.0f);

	constMap->mat = matWorld * matProjection;

	constBuff->Unmap(0, nullptr);

	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	device->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

	basicHeapHandle.ptr += descriptorHandleIncrementSize;

	srvDesc.Format = Dx12_Texture::GetTexResDesc(texName).Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		Dx12_Texture::GetTexBuff(texName).Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetCPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize)
	);
}

void KochaEngine::Texture2D::Draw()
{
	
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);

	matWorld = DirectX::XMMatrixIdentity();

	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate));
	matWorld *= DirectX::XMMatrixTranslation(position.x, position.y, 0);
	constMap->mat = matWorld * matProjection;
	constMap->color = this->color;
	constBuff->Unmap(0, nullptr);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize));
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::Texture2D::Draw(Vector2 position)
{
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);

	matWorld = DirectX::XMMatrixIdentity();

	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate));
	matWorld *= DirectX::XMMatrixTranslation(position.x, position.y, 0);
	constMap->mat = matWorld * matProjection;

	constBuff->Unmap(0, nullptr);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize));
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::Texture2D::AnimationDraw(UINT animationRate, Vector2 position)
{

	if (animationCount >= animationRate)
	{
		if (texNum > maxTex)
		{
			texNum = 1;
		}
		else
		{
			texNum++;
		}
		animationCount = 0;
		SetCutVertices(texNum);
	}
	else
	{
		animationCount++;
	}


	auto result = constBuff->Map(0, nullptr, (void**)&constMap);

	matWorld = DirectX::XMMatrixIdentity();

	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotate));
	matWorld *= DirectX::XMMatrixTranslation(position.x, position.y, 0);
	constMap->mat = matWorld * matProjection;

	constBuff->Unmap(0, nullptr);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { Dx12_Descriptor::GetHeap().Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(Dx12_Descriptor::GetHeap().Get()->GetGPUDescriptorHandleForHeapStart(), Dx12_Texture::GetTexNum(texName), descriptorHandleIncrementSize));
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::Texture2D::SetTexNum(UINT texNum)
{
	int a = 0;
	int b, c;

	for (UINT y = 0; y < cutY; y++)
	{
		for (UINT x = 0; x < cutX; x++)
		{
			a++;
			if (a == texNum)
			{
				b = x;
				c = y;
			}
		}
	}

	vertices[0] = { {0,size.y,0},{(float)b * cutSizeX,(float)c * cutSizeY + cutSizeY} };
	vertices[1] = { {0,0,0},{(float)b * cutSizeX,(float)c * cutSizeY} };
	vertices[2] = { {size.x,size.y,0},{(float)b * cutSizeX + cutSizeX,(float)c * cutSizeY + cutSizeY} };
	vertices[3] = { {size.x,0,0},{(float)b * cutSizeX + cutSizeX,(float)c * cutSizeY} };

	auto result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	vertBuff->Unmap(0, nullptr);
}
