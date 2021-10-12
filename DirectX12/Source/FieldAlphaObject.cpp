#include "../Header/FieldAlphaObject.h"

KochaEngine::FieldAlphaObject::FieldAlphaObject(Camera* arg_camera, const FieldAlphaObjType& arg_fieldAlphaObjType, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;

	camera = arg_camera;
	fieldAlphaObjType = arg_fieldAlphaObjType;
	position = arg_position;

	obj = new Object("plane");

	Initialize();
}

KochaEngine::FieldAlphaObject::~FieldAlphaObject()
{
	delete obj;
}

void KochaEngine::FieldAlphaObject::Initialize()
{
	isAlpha = true;
	switch (fieldAlphaObjType)
	{
	case KochaEngine::ROCK1:
		obj->SetTexture("Resources/rock1.png");
		obj->SetScale(Vector3(16, 16, 16));
		break;
	case KochaEngine::ROCK3:
		obj->SetTexture("Resources/rock3.png");
		obj->SetScale(Vector3(20, 20, 20));
		break;
	case KochaEngine::TREE1:
		obj->SetTexture("Resources/tree1.png");
		obj->SetScale(Vector3(30, 30, 30));
		break;
	case KochaEngine::TREE2:
		obj->SetTexture("Resources/tree2.png");
		obj->SetScale(Vector3(30, 30, 30));
		break;
	case KochaEngine::GRASS1:
		obj->SetTexture("Resources/grass1.png");
		obj->SetScale(Vector3(6, 6, 6));
		break;
	default:
		break;
	}
	obj->SetPosition(position);
	obj->SetBillboardType(Object::BILLBOARD_Y);
}

void KochaEngine::FieldAlphaObject::Update()
{
}

void KochaEngine::FieldAlphaObject::ObjDraw(Camera* camera, LightManager* arg_ligtManager)
{
	obj->Draw(camera, arg_ligtManager);
}
