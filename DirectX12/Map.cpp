#include "Map.h"
#include "Block.h"
#include "FieldObject.h"

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
			float posZ = -CHIP_SIZE * (float)z + (z_Data - 1) * 0.5f * CHIP_SIZE;
			float posX = CHIP_SIZE * (float)x - (x_Data - 1) * 0.5f * CHIP_SIZE;
			switch (mapData[z][x])
			{
			case 0:
				//gManager->AddObject(new FieldObject(camera, KochaEngine::FieldObjType::TREE, Vector3(posX, 0, posZ)));
				break;
			case 1:
				gManager->AddObject(new Block(KochaEngine::BlockType::SOIL_BLOCK, Vector3(posX, 8, posZ)));
				break;
			}
		}
	}
}
