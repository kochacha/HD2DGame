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
		unsigned short boneNo[2]; //�{�[���ԍ�
		unsigned char boneWeight; //�{�[���e���x
		unsigned char edgeFlag;   //�֊s���t���O
	};

#pragma pack(1) //1�o�C�g�p�b�L���O
	struct PMDMaterial
	{
		DirectX::XMFLOAT3 diffuse;  //�f�B�t���[�Y�F
		DirectX::XMFLOAT3 specular; //�X�y�L�����F
		DirectX::XMFLOAT3 ambient;  //�A���r�G���g�F
		float alpha;                //�f�B�t���[�Y��
		float specularity;          //�X�y�L�����̋���
		unsigned char toonIndex;    //�g�D�[���ԍ�
		unsigned char edgeFlag;     //�֊s���t���O
		unsigned int indicesNum;    //���̃}�e���A�������蓖�Ă���
		char texFilePath[20];       //�e�N�X�`���t�@�C���p�X�{��
	};
#pragma pack() //�p�b�L���O�w�������

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
