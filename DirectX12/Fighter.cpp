#include "Fighter.h"

KochaEngine::Fighter::Fighter(Camera* arg_camera, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	camera = arg_camera;

	position = position;

	Initialize();
}

KochaEngine::Fighter::~Fighter()
{
	//delete obj;
}

void KochaEngine::Fighter::Initialize()
{
	//パラメーターのセット(後々テキストデータから読み込めるようにする(セーブ機能))
	param.name = "Character/fighter.txt";
	param.texName = "fighter";
	param.attribute = TYPE_NORMAL;
	param.size = Vector3(10, 10, 10);
	param.level = 1;
	param.maxHP = 15;
	param.hp = 15;
	param.maxSP = 2;
	param.sp = 2;
	param.attack = 10;
	param.defence = 10;
	param.intelligence = 0;
	param.speed = 3;
	param.skillful = 6;
	param.luck = 0;
	param.exp = 0;
	param.money = 0;
}

void KochaEngine::Fighter::Update()
{
}

void KochaEngine::Fighter::Hit()
{
}

void KochaEngine::Fighter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
}

KochaEngine::GameObjectType KochaEngine::Fighter::GetType()
{
	return FIELD_FIGHTER;
}
