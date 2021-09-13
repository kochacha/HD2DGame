#include "FieldObject.h"


KochaEngine::FieldObject::FieldObject(Camera* arg_camera, const FieldObjType arg_fieldObjType, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;

	camera = arg_camera;
	fieldObjType = arg_fieldObjType;
	position = arg_position;

	switch (fieldObjType)
	{
	case KochaEngine::TREE:
		obj = new Object("LowTree");
		obj->SetScale(Vector3(2, 2, 2));
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

void KochaEngine::FieldObject::ObjDraw(Camera* camera, LightManager* arg_ligtManager)
{
	obj->Draw(camera, arg_ligtManager);
}
