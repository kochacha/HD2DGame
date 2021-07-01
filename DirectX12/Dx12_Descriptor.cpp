#include "Dx12_Descriptor.h"
#include "Dx12_Wrapper.h"

#define Release(X){if((X) != nullptr)(X)->Release();(X) = nullptr;}

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

ComPtr<ID3D12DescriptorHeap> KochaEngine::Dx12_Descriptor::heap{};
ComPtr<ID3D12DescriptorHeap> KochaEngine::Dx12_Descriptor::fbxHeap{};

KochaEngine::Dx12_Descriptor::Dx12_Descriptor(Dx12_Wrapper& dx12) : dx12(dx12)
{
	CreateHeap();
}

KochaEngine::Dx12_Descriptor::~Dx12_Descriptor()
{
	Release(heap);
	Release(fbxHeap);
}

HRESULT KochaEngine::Dx12_Descriptor::CreateHeap()
{
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 512;
		//ƒq[ƒv‚Ì¶¬
		result = dx12.GetDevice().Get()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&heap));
	}
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 512;
		//ƒq[ƒv‚Ì¶¬
		result = dx12.GetDevice().Get()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&fbxHeap));
	}
	return result;
}