#include "Dx12_Texture.h"
#include "Dx12_Wrapper.h"

static UINT count = 0;
static std::map<std::string, UINT> texNum;
static ComPtr<ID3D12Resource> texBuff[256];
static D3D12_RESOURCE_DESC texResDesc[256];
ID3D12Device* KochaEngine::Dx12_Texture::device{};

void KochaEngine::Dx12_Texture::LoadTexture(std::string arg_texName)
{
	//ComPtr<ID3D12Resource> a{};
	//D3D12_RESOURCE_DESC b{};
	if (device == nullptr) return;

	texNum.emplace(arg_texName, count);

	if (count < 256) { count++; }

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, arg_texName.c_str(), -1, wfilepath, _countof(wfilepath));

	//texBuff.emplace(texName, a[c]);
	//texResDesc.emplace(texName, b[c]);

	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	auto result = DirectX::LoadFromWICFile(
		wfilepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);

	texResDesc[texNum[arg_texName]].Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc[texNum[arg_texName]].Format = metadata.format;
	texResDesc[texNum[arg_texName]].Width = metadata.width;
	texResDesc[texNum[arg_texName]].Height = (UINT)metadata.height;
	texResDesc[texNum[arg_texName]].DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc[texNum[arg_texName]].MipLevels = (UINT16)metadata.mipLevels;
	texResDesc[texNum[arg_texName]].SampleDesc.Count = 1;

	D3D12_HEAP_PROPERTIES texHeapProp = {};

	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	result = device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc[texNum[arg_texName]],
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[texNum[arg_texName]]));


	result = texBuff[texNum[arg_texName]]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);
}

UINT KochaEngine::Dx12_Texture::GetTexNum(const std::string& arg_texName)
{
	return texNum[arg_texName];
}

ComPtr<ID3D12Resource> KochaEngine::Dx12_Texture::GetTexBuff(const std::string& arg_texName)
{ 
	return texBuff[texNum[arg_texName]];
}

D3D12_RESOURCE_DESC KochaEngine::Dx12_Texture::GetTexResDesc(const std::string& arg_texName)
{ 
	return texResDesc[texNum[arg_texName]];
}