#include "Map.h"
#include "Util.h"
#include "Block.h"
#include "FieldObject.h"
#include "FieldAlphaObject.h"
#include "Player.h"
#include "Fighter.h"
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
			case 100: //½àu©È¢
				
				break;
			case 101: //½è»è{bNX
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;

				//102Ô`199Ô \õ
			case 102:
				gManager->AddObject(new Block(KochaEngine::BlockType::SOIL_BLOCK, Vector3(posX, 8, posZ)));
				break;

				//200Ô`399Ô ÊíÌ3DIuWFNg c:¡}X
			case 200:  //ò(Ø)NX + »è 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 201: //ò(Ø)ã¤ 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 202: //ò(Ø)ã¤ + »è 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_DOWN));
				break;
			case 203: //ò(Ø)º¤ 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 204: //ò(Ø)º¤ + »è 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_UP));
				break;
			case 205: //ò(Ø)¶¤ 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 206: //ò(Ø)¶¤ + »è 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_RIGHT));
				break;
			case 207: //ò(Ø)E¤ 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 208: //ò(Ø)E¤ + »è 1:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_LEFT));
				break;
			case 209: //Ç(ÎK)¡ 1:5
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 210: //Ç(ÎK)¡ + »è 1:5
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 211: //Ç(ÎK)c 5:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 212: //Ç(ÎK)c + »è 5:1
				gManager->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;

				//400Ô`799Ô Â|ÌAt@IuWFNg
			case 400: //â1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				break;
			case 401: //â1 + »è
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 5));
				break;
			case 402: //Ø1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 12, posZ)));
				break;
			case 403: //Ø1 + »è
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 12, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 8));
				break;
			case 404: //ÍêØ1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 12, posZ)));
				break;
			case 405: //ÍêØ1 + »è
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 12, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 8));
				break;
			case 406: //æÎ
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				break;
			case 407: //æÎ + »è
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				gManager->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 4));
				break;
			case 408: //1
				gManager->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRASS1, Vector3(posX, 2, posZ)));
				break;
				//800Ô`999Ô NPCLN^[
			case 800:

				break;
			case 999: //LN^[
				gManager->AddObject(new Player(camera, gManager, Vector3(posX, 5, posZ)));
				gManager->AddObject(new Fighter(camera, Vector3(posX, 5, posZ)));
				break;
			default:
				break;
			}
		}
	}
}
