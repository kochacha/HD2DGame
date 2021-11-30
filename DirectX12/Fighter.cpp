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
	param.maxHP = 10;
	param.hp = 10;
	param.maxSP = 10;
	param.sp = 10;
	param.attack = 10;
	param.defence = 10;
	param.intelligence = 10;
	param.speed = 10;
	param.skillful = 10;
	param.luck = 10;
	param.exp = 0;
	param.money = 10;
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
