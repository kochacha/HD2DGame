#include "Map.h"
#include "Util.h"
#include "Block.h"
#include "FieldObject.h"
#include "FieldAlphaObject.h"
#include "Player.h"
#include "Fighter.h"
#include "CollisionBlock.h"
#include "SceneChangeBlock.h"
#include "SceneSpawnBlock.h"
#include "GameSetting.h"

KochaEngine::Map::Map(std::weak_ptr<GameObjectManager> arg_gManager, std::weak_ptr<Camera> arg_camera)
	:gManager(arg_gManager), camera(arg_camera)
{
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

	//マップチップの一番左上の数字によって環境が変わる(森林、砂漠、雪原、洞窟など)
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
			case 100: //何も置かない
				
				break;
			case 101: //当たり判定ボックス(中)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 102: //当たり判定ボックス(小)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7));
				break;
			case 103: //当たり判定ボックス(大)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15));
				break;
			case 104: //当たり判定ボックス上寄り(中)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_UP));
				break;
			case 105: //当たり判定ボックス下寄り(中)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_DOWN));
				break;
			case 106: //当たり判定ボックス左寄り(中)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_LEFT));
				break;
			case 107: //当たり判定ボックス右寄り(中)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_RIGHT));
				break;
			case 108: //当たり判定ボックス上寄り(小)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_UP));
				break;
			case 109: //当たり判定ボックス下寄り(小)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_DOWN));
				break;
			case 110: //当たり判定ボックス左寄り(小)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_LEFT));
				break;
			case 111: //当たり判定ボックス右寄り(小)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 3.5f, posZ), 7, KochaEngine::COLLISION_RIGHT));
				break;
			case 112: //当たり判定ボックス上寄り(大)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_UP));
				break;
			case 113: //当たり判定ボックス下寄り(大)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_DOWN));
				break;
			case 114: //当たり判定ボックス左寄り(大)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_LEFT));
				break;
			case 115: //当たり判定ボックス右寄り(大)
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 7.5f, posZ), 15, KochaEngine::COLLISION_RIGHT));
				break;

				//116番～199番 予備
			case 116:
				gManager.lock()->AddObject(new SceneChangeBlock(CHANGE_POINT_1, Vector3(posX, 5, posZ), 10));
				break;
			case 117:
				gManager.lock()->AddObject(new SceneChangeBlock(CHANGE_POINT_2, Vector3(posX, 5, posZ), 10));
				break;
			case 118:
				gManager.lock()->AddObject(new SceneChangeBlock(CHANGE_POINT_3, Vector3(posX, 5, posZ), 10));
				break;
			case 119:
				gManager.lock()->AddObject(new SceneChangeBlock(CHANGE_POINT_4, Vector3(posX, 5, posZ), 10));
				break;
			case 120:
				gManager.lock()->AddObject(new SceneChangeBlock(CHANGE_POINT_5, Vector3(posX, 5, posZ), 10));
				break;
			case 121:
				gManager.lock()->AddObject(new SceneSpawnBlock(SPAWN_POINT_1, Vector3(posX, 5, posZ), 10));
				break;
			case 122:
				gManager.lock()->AddObject(new SceneSpawnBlock(SPAWN_POINT_2, Vector3(posX, 5, posZ), 10));
				break;
			case 123:
				gManager.lock()->AddObject(new SceneSpawnBlock(SPAWN_POINT_3, Vector3(posX, 5, posZ), 10));
				break;
			case 124:
				gManager.lock()->AddObject(new SceneSpawnBlock(SPAWN_POINT_4, Vector3(posX, 5, posZ), 10));
				break;
			case 125:
				gManager.lock()->AddObject(new SceneSpawnBlock(SPAWN_POINT_5, Vector3(posX, 5, posZ), 10));
				break;

				//200番～399番 通常の3Dオブジェクト 縦:横マス
			case 200:  //柵(木)クロス + 判定 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 201: //柵(木)上側 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 202: //柵(木)上側 + 判定 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_DOWN));
				break;
			case 203: //柵(木)下側 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 204: //柵(木)下側 + 判定 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_UP));
				break;
			case 205: //柵(木)左側 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 206: //柵(木)左側 + 判定 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_RIGHT));
				break;
			case 207: //柵(木)右側 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 208: //柵(木)右側 + 判定 1:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SAKU_WOODEN_VERTICAL, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10, KochaEngine::COLLISION_LEFT));
				break;
			case 209: //壁(石レンガ)横 1:5
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				break;
			case 210: //壁(石レンガ)横 + 判定 1:5
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_SIDEWAYS, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 211: //壁(石レンガ)縦 5:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				break;
			case 212: //壁(石レンガ)縦 + 判定 5:1
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::WALL_SAND_VERTICAL, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 213: //小さい家(木)青屋根 + 判定 3:3
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SMALL_HOUSE_WOODEN_BLUE, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 40));
				break;
			case 214: //大きい家(木)青屋根 + 判定 3:3
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::LARGE_HOUSE_WOODEN_BLUE, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 35));
				break;
			case 215: //小さい家(木)赤屋根 + 判定 3:3
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::SMALL_HOUSE_WOODEN_RED, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 216: //大きい家(木)赤屋根 + 判定 3:3
				gManager.lock()->AddObject(new FieldObject(KochaEngine::FieldObjType::LARGE_HOUSE_WOODEN_RED, Vector3(posX, 0, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;

				//400番～799番 板ポリのアルファオブジェクト
			case 400: //岩1
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				break;
			case 401: //岩1 + 判定
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::ROCK1, Vector3(posX, 4, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 5));
				break;
			case 402: //木1
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 17, posZ)));
				break;
			case 403: //木1 + 判定
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::TREE1, Vector3(posX, 17, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 404: //枯れ木1
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 17, posZ)));
				break;
			case 405: //枯れ木1 + 判定
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::WITHER_TREE1, Vector3(posX, 17, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 10));
				break;
			case 406: //墓石
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				break;
			case 407: //墓石 + 判定
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRAVE_STONE, Vector3(posX, 4, posZ)));
				gManager.lock()->AddObject(new CollisionBlock(Vector3(posX, 5, posZ), 4));
				break;
			case 408: //草1
				gManager.lock()->AddObject(new FieldAlphaObject(KochaEngine::FieldAlphaObjType::GRASS1, Vector3(posX, 2, posZ)));
				break;
				//800番～999番 NPC等キャラクター
			case 800:

				break;
			case 999: //キャラクター
				//gManager.lock()->AddObject(new Player(gManager, Vector3(posX, 5, posZ)));
				//gManager.lock()->AddObject(new Fighter(Vector3(posX, 5, posZ)));
				break;
			default:
				break;
			}
		}
	}
}
