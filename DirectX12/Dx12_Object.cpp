#include "Dx12_Object.h"
#include "Dx12_Texture.h"
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <codecvt>
#include <d3dx12.h>

static UINT objCount = 0;
std::map<std::string, UINT> objNum;
static std::map<std::string, KochaEngine::Material> material;
static KochaEngine::Mesh mesh[256];
static std::map<std::string, D3D12_VERTEX_BUFFER_VIEW> vbViews;
static std::map<std::string, D3D12_INDEX_BUFFER_VIEW> ibViews;
KochaEngine::VertexPosNormalUv* vertMap = nullptr;
unsigned short* indexMap = nullptr;

void KochaEngine::Dx12_Object::LoadTexture(ID3D12Device* device, const std::string& directoryPath, const std::string& filename)
{
	//ファイルパスを結合
	std::string filepath = directoryPath + filename;

	Dx12_Texture::LoadTexture(device, filepath);
}

void KochaEngine::Dx12_Object::LoadMaterial(ID3D12Device* device, std::string objName, const std::string & directoryPath, const std::string & filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		//１行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());//先頭文字を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> material[objName].name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material[objName].ambient.x;
			line_stream >> material[objName].ambient.y;
			line_stream >> material[objName].ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material[objName].diffuse.x;
			line_stream >> material[objName].diffuse.y;
			line_stream >> material[objName].diffuse.z;
		}

		//先頭文字列がKdならスペキュラー色
		if (key == "Kd")
		{
			line_stream >> material[objName].specular.x;
			line_stream >> material[objName].specular.y;
			line_stream >> material[objName].specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> material[objName].textureFilename;
			LoadTexture(device, directoryPath, material[objName].textureFilename);
		}
	}
}

void KochaEngine::Dx12_Object::LoadObject(ID3D12Device* device, std::string objName)
{
	HRESULT result = S_FALSE;

	objNum.emplace(objName, objCount);
	vbViews.emplace(objName, mesh[objCount].vbView);
	ibViews.emplace(objName, mesh[objCount].ibView);

	std::ifstream file;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring ws = converter.from_bytes(objName);

	const std::string modelname(ws.begin(), ws.end());
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/Object/" + modelname + "/";
	material[objName].objFilename = filename;
	file.open(directoryPath + filename);

	if (file.fail())
	{
		assert(0);
	}

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;

	std::string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		std::string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			std::string filename;
			line_stream >> filename;

			LoadMaterial(device, objName, directoryPath, filename);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{
			//x,y,z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//座標データに追加
			positions.emplace_back(position);

			//頂点データに追加
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}

		if (key == "vt")
		{
			//U.V成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			//X.Y.Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//テクスチャ座標データに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				mesh[objNum[objName]].vertices.emplace_back(vertex);

				//インデックスデータの追加
				mesh[objNum[objName]].indices.emplace_back((unsigned short)mesh[objNum[objName]].indices.size());
			}
		}
	}
	file.close();

	CreateBufferView(device, objName);
	if (objCount < 256)
	{
		objCount++;
	}
}

void KochaEngine::Dx12_Object::CreateBufferView(ID3D12Device* device, std::string objName)
{
	auto sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * mesh[objNum[objName]].vertices.size());
	auto sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh[objNum[objName]].indices.size());
	D3D12_HEAP_PROPERTIES heapprop = {};
	D3D12_RESOURCE_DESC resdesc = {};

	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(mesh[objNum[objName]].vertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mesh[objNum[objName]].vertBuff));

	result = mesh[objNum[objName]].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 頂点バッファ生成
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mesh[objNum[objName]].vertBuff));

	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	auto heapProp2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc2 = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = device->CreateCommittedResource(
		&heapProp2,
		D3D12_HEAP_FLAG_NONE,
		&resDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mesh[objNum[objName]].indexBuff));

	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	result = mesh[objNum[objName]].vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(mesh[objNum[objName]].vertices.begin(), mesh[objNum[objName]].vertices.end(), vertMap);
		mesh[objNum[objName]].vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	result = mesh[objNum[objName]].indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(mesh[objNum[objName]].indices.begin(), mesh[objNum[objName]].indices.end(), indexMap);
		mesh[objNum[objName]].indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbViews[objName].BufferLocation = mesh[objNum[objName]].vertBuff->GetGPUVirtualAddress();
	vbViews[objName].SizeInBytes = sizeVB;
	vbViews[objName].StrideInBytes = sizeof(mesh[objNum[objName]].vertices[0]);

	// インデックスバッファビューの作成
	ibViews[objName].BufferLocation = mesh[objNum[objName]].indexBuff->GetGPUVirtualAddress();
	ibViews[objName].Format = DXGI_FORMAT_R16_UINT;
	ibViews[objName].SizeInBytes = sizeIB;

}

KochaEngine::Material KochaEngine::Dx12_Object::GetMaterial(const std::string& objName)
{
	return material[objName];
}

D3D12_VERTEX_BUFFER_VIEW KochaEngine::Dx12_Object::GetVBView(const std::string& objName)
{
	return vbViews[objName];
}

D3D12_INDEX_BUFFER_VIEW KochaEngine::Dx12_Object::GetIBView(const std::string& objName)
{
	return ibViews[objName];
}

std::vector<unsigned short> KochaEngine::Dx12_Object::GetIndices(const std::string& objName)
{
	return mesh[objNum[objName]].indices;
}
