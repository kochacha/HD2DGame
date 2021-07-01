#include "Dx12_Texture.h"
#include "Dx12_Wrapper.h"

static UINT count = 0;
static std::map<std::string, UINT> texNum;
static ComPtr<ID3D12Resource> texBuff[256];
static D3D12_RESOURCE_DESC texResDesc[256];

void KochaEngine::Dx12_Texture::LoadTexture(ID3D12Device* device, std::string texName)
{
	//ComPtr<ID3D12Resource> a{};
	//D3D12_RESOURCE_DESC b{};
	if (device == nullptr) return;

	texNum.emplace(texName, count);

	if (count < 256) { count++; }

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, texName.c_str(), -1, wfilepath, _countof(wfilepath));

	//texBuff.emplace(texName, a[c]);
	//texResDesc.emplace(texName, b[c]);

	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	auto result = DirectX::LoadFromWICFile(
		wfilepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);

	texResDesc[texNum[texName]].Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc[texNum[texName]].Format = metadata.format;
	texResDesc[texNum[texName]].Width = metadata.width;
	texResDesc[texNum[texName]].Height = (UINT)metadata.height;
	texResDesc[texNum[texName]].DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc[texNum[texName]].MipLevels = (UINT16)metadata.mipLevels;
	texResDesc[texNum[texName]].SampleDesc.Count = 1;

	D3D12_HEAP_PROPERTIES texHeapProp = {};

	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	result = device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc[texNum[texName]],
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[texNum[texName]]));


	result = texBuff[texNum[texName]]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);
}

UINT KochaEngine::Dx12_Texture::GetTexNum(const std::string& texName)
{
	return texNum[texName];
}

ComPtr<ID3D12Resource> KochaEngine::Dx12_Texture::GetTexBuff(const std::string& texName)
{ 
	return texBuff[texNum[texName]];
}

D3D12_RESOURCE_DESC KochaEngine::Dx12_Texture::GetTexResDesc(const std::string& texName)
{ 
	return texResDesc[texNum[texName]];
}