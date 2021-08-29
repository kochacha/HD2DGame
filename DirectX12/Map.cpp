#include "Map.h"
#include "Block.h"

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

	for (int x = 0; x < mapData[0].size(); x++)
	{
		for (int z = 0; z < mapData.size(); z++)
		{
			float posZ = -CHIP_SIZE * (float)z;
			float posX = CHIP_SIZE * (float)x;
			switch (mapData[z][x])
			{
			case 0:
				break;
			case 1:
				gManager->AddObject(new Block(camera, KochaEngine::BlockType::SOIL_BLOCK, Vector3(posX, 0, posZ)));
				break;
			}
		}
	}
}
