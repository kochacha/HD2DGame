#include "FieldObject.h"


KochaEngine::FieldObject::FieldObject(const FieldObjType arg_fieldObjType, const Vector3& arg_position)
{
	fieldObjType = arg_fieldObjType;
	position = arg_position;

	switch (fieldObjType)
	{
	case KochaEngine::TREE:
		obj = new Object("LowTree");
		obj->SetScale(Vector3(2, 2, 2));
		break;
	case KochaEngine::SAKU_WOODEN_VERTICAL:
		obj = new Object("saku_0");
		obj->SetScale(Vector3(13, 16, 13));
		obj->SetRotate(Vector3(0, 90, 0));
		break;
	case KochaEngine::SAKU_WOODEN_SIDEWAYS:
		obj = new Object("saku_0");
		obj->SetScale(Vector3(13, 16, 13));
		break;
	case KochaEngine::WALL_SAND_SIDEWAYS:
		obj = new Object("wall_0");
		obj->SetScale(Vector3(20, 20, 20));
		obj->SetRotate(Vector3(0, 90, 0));
		obj->SetTexture("Resources/Texture/Tiling/wall_1.png");
		break;
	case KochaEngine::WALL_SAND_VERTICAL:
		obj = new Object("wall_0");
		obj->SetScale(Vector3(20, 20, 20));
		obj->SetTexture("Resources/Texture/Tiling/wall_1.png");
		break;
	default:
		obj = new Object("box");
		obj->SetScale(Vector3(8, 8, 8));
		break;
	}
	Initialize();
}

KochaEngine::FieldObject::~FieldObject()
{
	delete obj;
}

void KochaEngine::FieldObject::Initialize()
{
	obj->SetPosition(position);
}

void KochaEngine::FieldObject::Update()
{
}

void KochaEngine::FieldObject::ObjDraw(Camera* arg_camera, LightManager* arg_ligtManager)
{
	obj->Draw(arg_camera, arg_ligtManager);
}
