#include "PostEffect.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Pipeline.h"
#include "Application.h"
#include "Input.h"
#include <d3dx12.h>

ID3D12Device* KochaEngine::PostEffect::device{};
ID3D12GraphicsCommandList* KochaEngine::PostEffect::cmdList{};
SIZE KochaEngine::PostEffect::winSize{};

KochaEngine::PostEffect::PostEffect()
{
	Initialize();
	SetVertices();
	CreateBufferView();
}

KochaEngine::PostEffect::~PostEffect()
{
}

//void KochaEngine::PostEffect::PreDrawShadow(ID3D12GraphicsCommandList* cmdList)
//{
//	//���\�[�X�o���A��ύX
//	for (int i = 0; i < TEX_BUFF_COUNT; i++)
//	{
//		auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
//			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
//			D3D12_RESOURCE_STATE_RENDER_TARGET);
//		cmdList->ResourceBarrier(1, &resBarrier);
//	}
//
//	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[TEX_BUFF_COUNT];
//	for (int i = 0; i < TEX_BUFF_COUNT; i++)
//	{
//		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
//			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
//	}
//
//	auto handle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
//	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
//
//	cmdList->OMSetRenderTargets(0, nullptr, false, &handle);
//
//	//�r���[�|�[�g�̐ݒ�
//	CD3DX12_VIEWPORT viewports[TEX_BUFF_COUNT];
//	//�V�U�����O��`�̐ݒ�
//	CD3DX12_RECT scissorRects[TEX_BUFF_COUNT];
//	for (int i = 0; i < TEX_BUFF_COUNT; i++)
//	{
//		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, winSize.cx, winSize.cy);
//		scissorRects[i] = CD3DX12_RECT(0, 0, winSize.cx, winSize.cy);
//	}
//	cmdList->RSSetViewports(TEX_BUFF_COUNT, viewports);
//	cmdList->RSSetScissorRects(TEX_BUFF_COUNT, scissorRects);
//
//	//�S��ʃN���A
//	for (int i = 0; i < TEX_BUFF_COUNT; i++)
//	{
//		cmdList->ClearRenderTargetView(rtvHs[i], Application::clearColor, 0, nullptr);
//	}
//	//�[�x�o�b�t�@�̃N���A
//	cmdList->ClearDepthStencilView(descHeapDSV->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
//}

void KochaEngine::PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &resBarrier);
	}

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[TEX_BUFF_COUNT];
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(TEX_BUFF_COUNT, rtvHs, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT viewports[TEX_BUFF_COUNT];
	//�V�U�����O��`�̐ݒ�
	CD3DX12_RECT scissorRects[TEX_BUFF_COUNT];
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, winSize.cx, winSize.cy);
		scissorRects[i] = CD3DX12_RECT(0, 0, winSize.cx, winSize.cy);
	}
	cmdList->RSSetViewports(TEX_BUFF_COUNT, viewports);
	cmdList->RSSetScissorRects(TEX_BUFF_COUNT, scissorRects);

	//�S��ʃN���A
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], Application::clearColor, 0, nullptr);
	}
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void KochaEngine::PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		cmdList->ResourceBarrier(1, &resBarrier);
	}
}

void KochaEngine::PostEffect::Draw()
{
	time++;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->value = value;
	constMap->time = time;
	constBuff->Unmap(0, nullptr);

	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetPeraRootSignature().Get());
	cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &vertBuffView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, 
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 2,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//�[�x�o�b�t�@�e�N�X�`��
	cmdList->SetDescriptorHeaps(1, _depthSRVHeap.GetAddressOf());
	//auto handle = _depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	//handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, _depthSRVHeap->GetGPUDescriptorHandleForHeapStart());

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::PostEffect::Draw(const ShaderType& arg_type)
{
	time++;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->value = value;
	constMap->time = time;
	constBuff->Unmap(0, nullptr);

	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetPeraRootSignature().Get());

	switch (arg_type)
	{
	case ShaderType::PERA_SHADER: //�ʏ�
		cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());
		break;
	case ShaderType::VIGNETTE_SHADER: //�r�l�b�g
		cmdList->SetPipelineState(Dx12_Pipeline::vignettePipelineState.Get());
		break;
	case ShaderType::BLOOM_SHADER: //�u���[��
		cmdList->SetPipelineState(Dx12_Pipeline::bloomPipelineState.Get());
		break;
	case ShaderType::GAME_BOY_SHADER: //�Q�[���{�[�C��
		cmdList->SetPipelineState(Dx12_Pipeline::gameBoyPipelineState.Get());
		break;
	case ShaderType::CHROMATIC_ABERRATION_SHADER: //�F����
		cmdList->SetPipelineState(Dx12_Pipeline::cAbePipelineState.Get());
		break;
	case ShaderType::TOON_SHADER: //�K��
		cmdList->SetPipelineState(Dx12_Pipeline::toonPipelineState.Get());
		break;
	case ShaderType::GRAY_SCALE_SHADER: //�O���[�X�P�[��
		cmdList->SetPipelineState(Dx12_Pipeline::grayScalePipelineState.Get());
		break;
	case ShaderType::MOSAIC_SHADER: //���U�C�N
		cmdList->SetPipelineState(Dx12_Pipeline::mosaicPipelineState.Get());
		break;
	case ShaderType::GAUSSIAN_BLUR_SHADER: //�K�E�V�A���u���[
		cmdList->SetPipelineState(Dx12_Pipeline::blurPipelineState.Get());
		break;
	default: //�ʏ�
		cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());
		break;
	}

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &vertBuffView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 2,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//�[�x�o�b�t�@�e�N�X�`��
	cmdList->SetDescriptorHeaps(1, _depthSRVHeap.GetAddressOf());
	auto handle = _depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, handle);

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::PostEffect::StaticInit(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const SIZE winSize)
{
	KochaEngine::PostEffect::winSize = winSize;

	if (device == nullptr) return;
	KochaEngine::PostEffect::device = device;

	if (cmdList == nullptr) return;
	KochaEngine::PostEffect::cmdList = cmdList;
}

void KochaEngine::PostEffect::Initialize()
{
	color = Vector4(1, 1, 1, 1);
	time = 0;
	value = 0;

	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		winSize.cx,
		(UINT)winSize.cy,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//�e�N�X�`���o�b�t�@�̐���
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, Application::clearColor);
		for (int i = 0; i < TEX_BUFF_COUNT; i++)
		{
			result = device->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&texDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&texBuff[i]));
			assert(SUCCEEDED(result));

			//��f��
			const UINT pixelCount = winSize.cx * winSize.cy;
			//�摜�ꌅ���̃f�[�^�T�C�Y
			const UINT rowPitch = sizeof(UINT) * winSize.cx;
			//�摜�S�̂̃f�[�^�T�C�Y
			const UINT depthPitch = rowPitch * winSize.cy;
			//�摜�C���[�W
			UINT* img = new UINT[pixelCount];
			for (int j = 0; j < pixelCount; j++)
			{
				img[j] = 0xff0000ff;
			}

			//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}

	}

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = TEX_BUFF_COUNT;

	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		device->CreateShaderResourceView(texBuff[i].Get(), &srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = TEX_BUFF_COUNT;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��RTV�쐬
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		device->CreateRenderTargetView(texBuff[i].Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(),i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		winSize.cx,
		winSize.cy,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x�o�b�t�@�̐���
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&depthBuff));
		assert(SUCCEEDED(result));
	}
	{
		depthResDesc.Width = winSize.cx / 200;
		depthResDesc.Height = winSize.cy / 200;
		depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(lightDepthBuff.ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(result));
	}

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 2;

	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		auto handle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

		device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		device->CreateDepthStencilView(lightDepthBuff.Get(), &dsvDesc, handle);
	
	}
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 2;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_depthSRVHeap));
	}
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC resDesc = {};
		resDesc.Format = DXGI_FORMAT_R32_FLOAT;
		resDesc.Texture2D.MipLevels = 1;
		resDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		resDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		auto handle = _depthSRVHeap->GetCPUDescriptorHandleForHeapStart();

		device->CreateShaderResourceView(depthBuff.Get(), &resDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		device->CreateShaderResourceView(lightDepthBuff.Get(), &resDesc, handle);
	}
}

void KochaEngine::PostEffect::SetVertices()
{
	vertex[0] = { {-1, -1, 0.1f}, {0, 1} }; //����
	vertex[1] = { {-1,  1, 0.1f}, {0, 0} }; //����
	vertex[2] = { { 1, -1, 0.1f}, {1, 1} }; //�E��
	vertex[3] = { { 1,  1, 0.1f}, {1, 0} }; //�E��
}

void KochaEngine::PostEffect::CreateBufferView()
{
	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex));
	auto result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));

	vertBuffView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertBuffView.SizeInBytes = sizeof(vertex);
	vertBuffView.StrideInBytes = sizeof(PeraVartex);

	PeraVartex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertex), std::end(vertex), vertMap);
	vertBuff->Unmap(0, nullptr);

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
			IID_PPV_ARGS(&constBuff));
	}
}
