#pragma once
#include <string>
#include <vector>
#include <map>
#include <d3d12.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace KochaEngine
{

	struct PMDHeader
	{
		float version;
		char modelName[20];
		char comment[256];
	};

	struct PMDVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		unsigned short boneNo[2]; //ボーン番号
		unsigned char boneWeight; //ボーン影響度
		unsigned char edgeFlag;   //輪郭線フラグ
	};

#pragma pack(1) //1バイトパッキング
	struct PMDMaterial
	{
		DirectX::XMFLOAT3 diffuse;  //ディフューズ色
		DirectX::XMFLOAT3 specular; //スペキュラ色
		DirectX::XMFLOAT3 ambient;  //アンビエント色
		float alpha;                //ディフューズα
		float specularity;          //スペキュラの強さ
		unsigned char toonIndex;    //トゥーン番号
		unsigned char edgeFlag;     //輪郭線フラグ
		unsigned int indicesNum;    //このマテリアルが割り当てられる
		char texFilePath[20];       //テクスチャファイルパス＋α
	};
#pragma pack() //パッキング指定を解除

	struct PMDMesh
	{
		std::vector<unsigned short> indices;
		D3D12_VERTEX_BUFFER_VIEW vbView;
		D3D12_INDEX_BUFFER_VIEW ibView;
		ComPtr<ID3D12Resource> vertBuff;
		ComPtr<ID3D12Resource> indexBuff;
		std::vector<PMDMaterial> pmdMaterials;
	};

	class PMDLoader
	{
	private:


	public:

		static void LoadModel(ID3D12Device* device, std::string pmdName);
		static void LoadMaterial();

		static std::vector<unsigned short> GetIndices(std::string pmdName);
		static PMDMesh* GetPMDMesh(std::string pmdName);
	};
}
