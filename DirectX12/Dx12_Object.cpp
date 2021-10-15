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
ID3D12Device* KochaEngine::Dx12_Object::device{};

void KochaEngine::Dx12_Object::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���p�X������
	std::string filepath = directoryPath + filename;

	Dx12_Texture::LoadTexture(filepath);
}

void KochaEngine::Dx12_Object::LoadMaterial(std::string objName, const std::string & directoryPath, const std::string & filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		//�P�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin());//�擪�������폜
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			//�}�e���A�����ǂݍ���
			line_stream >> material[objName].name;
		}

		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> material[objName].ambient.x;
			line_stream >> material[objName].ambient.y;
			line_stream >> material[objName].ambient.z;
		}

		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> material[objName].diffuse.x;
			line_stream >> material[objName].diffuse.y;
			line_stream >> material[objName].diffuse.z;
		}

		//�擪������Kd�Ȃ�X�y�L�����[�F
		if (key == "Kd")
		{
			line_stream >> material[objName].specular.x;
			line_stream >> material[objName].specular.y;
			line_stream >> material[objName].specular.z;
		}

		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material[objName].textureFilename;
			LoadTexture(directoryPath, material[objName].textureFilename);
		}
	}
}

void KochaEngine::Dx12_Object::LoadObject(std::string objName)
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

			LoadMaterial(objName, directoryPath, filename);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			//x,y,z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

			//���_�f�[�^�ɒǉ�
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}

		if (key == "vt")
		{
			//U.V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			//X.Y.Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); //�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				mesh[objNum[objName]].vertices.emplace_back(vertex);

				//�C���f�b�N�X�f�[�^�̒ǉ�
				mesh[objNum[objName]].indices.emplace_back((unsigned short)mesh[objNum[objName]].indices.size());
			}
		}
	}
	file.close();

	CreateBufferView(objName);
	if (objCount < 256)
	{
		objCount++;
	}
}

void KochaEngine::Dx12_Object::CreateBufferView(std::string objName)
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

	// ���_�o�b�t�@����
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

	// �C���f�b�N�X�o�b�t�@����
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

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	result = mesh[objNum[objName]].vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(mesh[objNum[objName]].vertices.begin(), mesh[objNum[objName]].vertices.end(), vertMap);
		mesh[objNum[objName]].vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	result = mesh[objNum[objName]].indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(mesh[objNum[objName]].indices.begin(), mesh[objNum[objName]].indices.end(), indexMap);
		mesh[objNum[objName]].indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbViews[objName].BufferLocation = mesh[objNum[objName]].vertBuff->GetGPUVirtualAddress();
	vbViews[objName].SizeInBytes = sizeVB;
	vbViews[objName].StrideInBytes = sizeof(mesh[objNum[objName]].vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
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
