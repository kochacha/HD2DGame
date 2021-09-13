#include "Map.h"
#include "Util.h"
#include "Block.h"
#include "FieldObject.h"
#include "FieldAlphaObject.h"
#include "Player.h"
#include "CollisionBlock.h"

KochaEngine::Map::Map(GameObjectManager* arg_gManager, Camera* arg_camera)
{
	if (arg_gManager == nullptr) return;
	if (arg_camera == nullptr) return;
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

	for (int x = 0; x < x_Data; x++)
	{
		for (int z = 0; z < z_Data; z++)
		{
			//auto randZ = (float)(Util::GetIntRand(0,100)) * 0.10f;
			float posZ = -CHIP_SIZE * (float)z + (z_Data - 1) * 0.5f * CHIP_SIZE;
			float posX = CHIP_SIZE * (float)x - (x_Data - 1) * 0.5f * CHIP_SIZE;
			switch (mapData[z][x])
			{
			case 10:
				
				break;
			case 11:
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ)));
				break;
			case 12:
				gManager->AddObject(new Block(KochaEngine::BlockType::SOIL_BLOCK, Vector3(posX, 8, posZ)));
				break;
			case 30:
				gManager->AddObject(new FieldObject(camera, KochaEngine::FieldObjType::TREE, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ)));
				break;
			case 60:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 8, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ)));
				break;
			case 61:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::ROCK2, Vector3(posX, 10, posZ)));
				break;
			case 62:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::ROCK3, Vector3(posX, 4, posZ)));
				break;
			case 63:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 15, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ)));
				break;
			case 64:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::TREE2, Vector3(posX, 15, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ)));
				break;
			case 65:
				break;
			case 66:
				gManager->AddObject(new FieldAlphaObject(camera, KochaEngine::FieldAlphaObjType::GRASS1, Vector3(posX, 4, posZ)));
				break;
			case 99:
				gManager->AddObject(new Player(camera, gManager, Vector3(posX, 5, posZ)));
				break;
			default:
				break;
			}
		}
	}
}
