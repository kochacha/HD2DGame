#pragma once
#include <string>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <fbxsdk.h>

namespace KochaEngine
{
	struct Node
	{
		//���O
		std::string name;
		//���[�J���X�P�[��
		DirectX::XMVECTOR scaling = { 1,1,1,0 };
		//���[�J����]�p
		DirectX::XMVECTOR rotation = { 0,0,0,0 };
		//���[�J���ړ�
		DirectX::XMVECTOR translation = { 0,0,0,1 };
		//���[�J���ό`�s��
		DirectX::XMMATRIX transform;
		//�O���[�o���ό`�s��
		DirectX::XMMATRIX globalTransform;
		//�e�m�[�h
		Node* parent = nullptr;
	};

	class FBXModel
	{
	public:
		friend class FBXLoader;

		//�{�[���C���f�b�N�X�̍ő吔
		static const int MAX_BONE_INDICES = 4;

	public:
		//���_�f�[�^�\����
		struct VertexPosNormalUvSkin
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
			UINT boneIndex[MAX_BONE_INDICES];
			float boneWeight[MAX_BONE_INDICES];
		};

		//�{�[���\����
		struct Bone
		{
			//���O
			std::string name;
			//�����p���̋t�s��
			DirectX::XMMATRIX invInitialPose;
			//�N���X�^�[(FBX���̃{�[�����)
			FbxCluster* fbxCluster;
			//�R���X�g���N�^
			Bone(const std::string& name)
			{
				this->name = name;
			}
		};

		FBXModel();
		~FBXModel();

		//�`��
		void Draw(ID3D12GraphicsCommandList* cmdList);
		//���f���̕ϊ��s��擾
		const DirectX::XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

		std::vector<Bone>& GetBones() { return bones; }
		FbxScene* GetFbxScene() { return fbxScene; }

	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		std::string name;
		std::vector<Node> nodes;
		//�{�[���z��
		std::vector<Bone> bones;

		FbxScene* fbxScene = nullptr;

		//���b�V�������m�[�h
		Node* meshNode = nullptr;
		//���_�f�[�^�z��
		std::vector<VertexPosNormalUvSkin> vertices;
		//���_�C���f�b�N�X�z��
		std::vector<unsigned short> indices;

		//���_�o�b�t�@
		ComPtr<ID3D12Resource> vertBuff;
		//���_�o�b�t�@
		ComPtr<ID3D12Resource> indexBuff;
		//���_�o�b�t�@
		ComPtr<ID3D12Resource> texBuff;
		//���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		//�C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		//�A���r�G���g�W��
		DirectX::XMFLOAT3 ambient = { 1,1,1 };
		//�f�B�t���[�Y�W��
		DirectX::XMFLOAT3 diffuse = { 1,1,1 };
		//�e�N�X�`�����^�f�[�^
		DirectX::TexMetadata metadata = {};
		//�X�N���b�`�C���[�W
		DirectX::ScratchImage scratchImg = {};

		void CreateBuffers(ID3D12Device* device);
	};
}
