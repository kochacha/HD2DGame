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
	case KochaEngine::SAKU_WOODEN_SIDEWAYS_UP:
		position.z += 5;
		obj = new Object("saku");
		obj->SetScale(Vector3(0.73f, 1.25f, 1));
		break;
	case KochaEngine::SAKU_WOODEN_SIDEWAYS_DOWN:
		position.z -= 5;
		obj = new Object("saku");
		obj->SetScale(Vector3(0.73f, 1.25f, 1));
		break;
	case KochaEngine::SAKU_WOODEN_VERTICAL_LEFT:
		position.x -= 5;
		obj = new Object("saku");
		obj->SetScale(Vector3(0.73f, 1.25f, 1));
		obj->SetRotate(Vector3(0, 90, 0));
		break;
	case KochaEngine::SAKU_WOODEN_VERTICAL_RIGHT:
		position.x += 5;
		obj = new Object("saku");
		obj->SetScale(Vector3(0.73f, 1.25f, 1));
		obj->SetRotate(Vector3(0, 90, 0));
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
