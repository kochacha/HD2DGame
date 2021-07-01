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
		//名前
		std::string name;
		//ローカルスケール
		DirectX::XMVECTOR scaling = { 1,1,1,0 };
		//ローカル回転角
		DirectX::XMVECTOR rotation = { 0,0,0,0 };
		//ローカル移動
		DirectX::XMVECTOR translation = { 0,0,0,1 };
		//ローカル変形行列
		DirectX::XMMATRIX transform;
		//グローバル変形行列
		DirectX::XMMATRIX globalTransform;
		//親ノード
		Node* parent = nullptr;
	};

	class FBXModel
	{
	public:
		friend class FBXLoader;

		//ボーンインデックスの最大数
		static const int MAX_BONE_INDICES = 4;

	public:
		//頂点データ構造体
		struct VertexPosNormalUvSkin
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
			UINT boneIndex[MAX_BONE_INDICES];
			float boneWeight[MAX_BONE_INDICES];
		};

		//ボーン構造体
		struct Bone
		{
			//名前
			std::string name;
			//初期姿勢の逆行列
			DirectX::XMMATRIX invInitialPose;
			//クラスター(FBX側のボーン情報)
			FbxCluster* fbxCluster;
			//コンストラクタ
			Bone(const std::string& name)
			{
				this->name = name;
			}
		};

		FBXModel();
		~FBXModel();

		//描画
		void Draw(ID3D12GraphicsCommandList* cmdList);
		//モデルの変換行列取得
		const DirectX::XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

		std::vector<Bone>& GetBones() { return bones; }
		FbxScene* GetFbxScene() { return fbxScene; }

	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		std::string name;
		std::vector<Node> nodes;
		//ボーン配列
		std::vector<Bone> bones;

		FbxScene* fbxScene = nullptr;

		//メッシュを持つノード
		Node* meshNode = nullptr;
		//頂点データ配列
		std::vector<VertexPosNormalUvSkin> vertices;
		//頂点インデックス配列
		std::vector<unsigned short> indices;

		//頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		//頂点バッファ
		ComPtr<ID3D12Resource> indexBuff;
		//頂点バッファ
		ComPtr<ID3D12Resource> texBuff;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		//インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		//アンビエント係数
		DirectX::XMFLOAT3 ambient = { 1,1,1 };
		//ディフューズ係数
		DirectX::XMFLOAT3 diffuse = { 1,1,1 };
		//テクスチャメタデータ
		DirectX::TexMetadata metadata = {};
		//スクラッチイメージ
		DirectX::ScratchImage scratchImg = {};

		void CreateBuffers(ID3D12Device* device);
	};
}
