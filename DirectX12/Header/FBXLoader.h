#pragma once

#include "fbxsdk.h"
#include "FBXModel.h"

#include <string>
#include <d3d12.h>
#include <d3dx12.h>

namespace KochaEngine
{
	class FBXLoader
	{
	public:
		/// <summary>
		/// シングルトンインスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static FBXLoader* GetInstance();

		void Initialize(ID3D12Device* device);

		void Finalize();

		//ファイルからFBXモデル読み込み
		FBXModel* LoadModelFromFile(const std::string& modelName);

		static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

		//モデル格納ルートパス
		static const std::string baseDirectory;

		//テクスチャが無い場合の標準テクスチャファイル名
		static const std::string defaultTextureFileName;

	private:
		// privateなコンストラクタ（シングルトンパターン）
		FBXLoader() = default;
		// privateなデストラクタ（シングルトンパターン）
		~FBXLoader() = default;
		// コピーコンストラクタを禁止（シングルトンパターン）
		FBXLoader(const FBXLoader& obj) = delete;
		// コピー代入演算子を禁止（シングルトンパターン）
		void operator=(const FBXLoader& obj) = delete;


		//再帰的にノード構成を解析
		void ParseNodeRecursively(FBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);

		//メッシュ読み取り
		void ParseMesh(FBXModel* model, FbxNode* fbxNode);

		//頂点座標読み取り
		void ParseMeshVertices(FBXModel* model, FbxMesh* fbxMesh);

		//面情報読み取り
		void ParseMeshFaces(FBXModel* model, FbxMesh* fbxMesh);

		//マテリアル読み取り
		void ParseMaterial(FBXModel* model, FbxNode* fbxNode);

		//テクスチャ読み込み
		void LoadTexture(FBXModel* model, const std::string& fullpath);

		//スキニング情報の読み取り
		void ParseSkin(FBXModel* model, FbxMesh* mesh);

		//ディレクトリを含んだファイルパスから
		std::string ExtractFileName(const std::string& path);


		//D3D12デバイス
		ID3D12Device* device = nullptr;
		//FBXマネージャ
		FbxManager* fbxManager = nullptr;
		//FBXインポータ
		FbxImporter* fbxImporter = nullptr;
	};
}