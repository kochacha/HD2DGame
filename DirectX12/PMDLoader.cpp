#include "PMDLoader.h"
#include "Util.h"
#include <d3dx12.h>

static UINT pmdCount = 0;
std::map<std::string, UINT> pmdNum;
static KochaEngine::PMDMesh pmdMesh[64];

void KochaEngine::PMDLoader::LoadModel(ID3D12Device* device, std::string pmdName)
{
    pmdNum.emplace(pmdName, pmdCount);

    unsigned char* vertMap = nullptr;
    unsigned short* indexMap = nullptr;

    PMDHeader pmdHeader = {};
    char signature[3] = {};
    FILE* pf;
    auto result = fopen_s(&pf, pmdName.c_str(), "rb");

    result = fread(signature, sizeof(signature), 1, pf);
    result = fread(&pmdHeader, sizeof(pmdHeader), 1, pf);

    constexpr size_t pmdvertex_size = 38; //頂点一つ当たりのサイズ

    unsigned int vertNum = 0;
    fread(&vertNum, sizeof(vertNum), 1, pf);

    std::vector<unsigned char> vertices(vertNum * pmdvertex_size);
    fread(vertices.data(), vertices.size(), 1, pf);

    //インデックス数の読み込み
    unsigned int indicesNum = 0;
    fread(&indicesNum, sizeof(indicesNum), 1, pf);

    //インデックスデータの読み込み
    pmdMesh[pmdCount].indices.resize(indicesNum);
    fread(pmdMesh[pmdCount].indices.data(), pmdMesh[pmdCount].indices.size() * sizeof(pmdMesh[pmdCount].indices[0]), 1, pf);

    unsigned int materialNum; //マテリアル数
    fread(&materialNum, sizeof(materialNum), 1, pf);

    pmdMesh[pmdCount].pmdMaterials.resize(materialNum);
    fread(pmdMesh[pmdCount].pmdMaterials.data(), pmdMesh[pmdCount].pmdMaterials.size() * sizeof(PMDMaterial), 1, pf);

    fclose(pf);

    //頂点バッファーの作成
    {
        auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(vertices.size());
        result = device->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&pmdMesh[pmdCount].vertBuff));
    }

    pmdMesh[pmdCount].vbView.BufferLocation = pmdMesh[pmdCount].vertBuff->GetGPUVirtualAddress();
    pmdMesh[pmdCount].vbView.SizeInBytes = vertices.size();
    pmdMesh[pmdCount].vbView.StrideInBytes = pmdvertex_size;

    result = pmdMesh[pmdCount].vertBuff->Map(0, nullptr, (void**)&vertMap);
    std::copy(std::begin(vertices), std::end(vertices), vertMap);
    pmdMesh[pmdCount].vertBuff->Unmap(0, nullptr);

    //インデックスバッファーの作成
    {
        auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(pmdMesh[pmdCount].indices.size() * sizeof(pmdMesh[pmdCount].indices[0]));
        result = device->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&pmdMesh[pmdCount].indexBuff));
    }

    pmdMesh[pmdCount].ibView.BufferLocation = pmdMesh[pmdCount].indexBuff->GetGPUVirtualAddress();
    pmdMesh[pmdCount].ibView.Format = DXGI_FORMAT_R16_UINT;
    pmdMesh[pmdCount].ibView.SizeInBytes = pmdMesh[pmdCount].indices.size() * sizeof(pmdMesh[pmdCount].indices[0]);

    result = pmdMesh[pmdCount].indexBuff->Map(0, nullptr, (void**)&indexMap);
    
    std::copy(std::begin(pmdMesh[pmdCount].indices), std::end(pmdMesh[pmdCount].indices), indexMap);
    
    pmdMesh[pmdCount].indexBuff->Unmap(0, nullptr);


    if (pmdCount < 64)
    {
        pmdCount++;
    }
}

void KochaEngine::PMDLoader::LoadMaterial()
{

}

std::vector<unsigned short> KochaEngine::PMDLoader::GetIndices(std::string pmdName)
{
    return pmdMesh[pmdNum[pmdName]].indices;
}

KochaEngine::PMDMesh* KochaEngine::PMDLoader::GetPMDMesh(std::string pmdName)
{
    return &pmdMesh[pmdNum[pmdName]];
}
