#pragma once
#include <d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

namespace KochaEngine
{
	class Dx12_Wrapper;
	class Dx12_Descriptor
	{
	private:

		static ComPtr<ID3D12DescriptorHeap> heap;
		static ComPtr<ID3D12DescriptorHeap> fbxHeap;

		HRESULT CreateHeap();

		HRESULT result;

		Dx12_Wrapper& dx12;

	public:

		Dx12_Descriptor(Dx12_Wrapper& dx12);

		~Dx12_Descriptor();

		static ComPtr<ID3D12DescriptorHeap> GetHeap() { return heap; }
		static ComPtr<ID3D12DescriptorHeap> GetFBXHeap() { return fbxHeap; }
	};
}