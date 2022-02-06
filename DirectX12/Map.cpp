#include "Map.h"
#include "Util.h"
#include "Block.h"
#include "FieldObject.h"
#include "FieldAlphaObject.h"
#include "Player.h"
#include "Fighter.h"
#include "CollisionBlock.h"
#include "SceneChangeBlock.h"
#include "GameSetting.h"

KochaEngine::Map::Map(GameObjectManager* arg_gManager, Camera* arg_camera)
{
	if (!arg_gManager) return;
	if (!arg_camera) return;
	gManager = arg_gManager;
	camera = arg_camera;
}

KochaEngine::Map::~Map()
{
}

void KochaEngine::Map::CreateMap(const int arg_mapNum)
{
	char fileName[64];
	sprintf_s(fileName, 64, "Resources/Map/Field%d.csv", arg_mapNum);
	csvReader.LoadCSV(arg_mapNum, fileName);
	mapData = csvReader.GetMapData(arg_mapNum);

	auto x_Data = mapData[0].size();
	auto z_Data = mapData.size();

	//�}�b�v�`�b�v�̈�ԍ���̐����ɂ���Ċ����ς��(�X�сA�����A�ጴ�A���A�Ȃ�)
	GameSetting::environmentNumber = mapData[0][0];
	GameSetting::isEnvironmentUpdate = true;

	for (int x = 0; x < x_Data; x++)
	{
		for (int z = 0; z < z_Data; z++)
		{
			float posZ = -CHIP_SIZE * (float)z + (z_Data - 1) * 0.5f * CHIP_SIZE;
			float posX = CHIP_SIZE * (float)x - (x_Data - 1) * 0.5f * CHIP_SIZE;
			switch (mapData[z][x])
			{
			case 100: //�����u���Ȃ�
				
				break;
			case 101: //�����蔻��{�b�N�X(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 102: //�����蔻��{�b�N�X(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7));
				break;
			case 103: //�����蔻��{�b�N�X(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15));
				break;
			case 104: //�����蔻��{�b�N�X����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_UP));
				break;
			case 105: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_DOWN));
				break;
			case 106: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_LEFT));
				break;
			case 107: //�����蔻��{�b�N�X�E���(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_RIGHT));
				break;
			case 108: //�����蔻��{�b�N�X����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_UP));
				break;
			case 109: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_DOWN));
				break;
			case 110: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_LEFT));
				break;
			case 111: //�����蔻��{�b�N�X�E���(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_RIGHT));
				break;
			case 112: //�����蔻��{�b�N�X����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_UP));
				break;
			case 113: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_DOWN));
				break;
			case 114: //�����蔻��{�b�N�X�����(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_LEFT));
				break;
			case 115: //�����蔻��{�b�N�X�E���(��)
				gManager->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_RIGHT));
				break;

				//116�ԁ`199�� �\��
			case 116:
				gManager->AddObject(new Block(KochaEngine::BlockType::SOIL_BLOCK, Vector3(posX, 8, posZ)));
				break;

				//200�ԁ`399�� �ʏ��3D�I�u�W�F�N�g �c:���}�X
			case 200:  //��(��)�N���X + ���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 201: //��(��)�㑤 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 202: //��(��)�㑤 + ���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_DOWN));
				break;
			case 203: //��(��)���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 204: //��(��)���� + ���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_UP));
				break;
			case 205: //��(��)���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 206: //��(��)���� + ���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_RIGHT));
				break;
			case 207: //��(��)�E�� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 208: //��(��)�E�� + ���� 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_LEFT));
				break;
			case 209: //��(�΃����K)�� 1:5
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 210: //��(�΃����K)�� + ���� 1:5
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 211: //��(�΃����K)�c 5:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 212: //��(�΃����K)�c + ���� 5:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 213: //��������(��)���� + ���� 3:3
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SMALL_HOUSE_WOODEN_BLUE, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 40));
				break;
			case 214: //�傫����(��)���� + ���� 3:3
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::LARGE_HOUSE_WOODEN_BLUE, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 35));
				break;
			case 215: //��������(��)�ԉ��� + ���� 3:3
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SMALL_HOUSE_WOODEN_RED, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 216: //�傫����(��)�ԉ��� + ���� 3:3
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::LARGE_HOUSE_WOODEN_RED, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;

				//400�ԁ`799�� �|���̃A���t�@�I�u�W�F�N�g
			case 400: //��1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				break;
			case 401: //��1 + ����
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 5));
				break;
			case 402: //��1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 17, posZ)));
				break;
			case 403: //��1 + ����
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 17, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 404: //�͂��1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 17, posZ)));
				break;
			case 405: //�͂��1 + ����
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 17, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 406: //���
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				break;
			case 407: //��� + ����
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 4));
				break;
			case 408: //��1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRASS1, Vector3(posX, 2, posZ)));
				break;
				//800�ԁ`999�� NPC���L�����N�^�[
			case 800:

				break;
			case 999: //�L�����N�^�[
				gManager->AddObject(new Player(camera, gManager, Vector3(posX, 5, posZ)));
				gManager->AddObject(new Fighter(camera, Vector3(posX, 5, posZ)));
				break;
			default:
				break;
			}
		}
	}
}
