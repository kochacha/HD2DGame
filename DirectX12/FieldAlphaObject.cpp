#include "FieldAlphaObject.h"

KochaEngine::FieldAlphaObject::FieldAlphaObject(const FieldAlphaObjType& arg_fieldAlphaObjType, const Vector3& arg_position)
{
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
		obj->SetTexture("Resources/Texture/Object/rock1.png");
		obj->SetScale(Vector3(8, 8, 8));
		break;
	case KochaEngine::ROCK3:
		obj->SetTexture("Resources/Texture/Object/rock3.png");
		obj->SetScale(Vector3(15, 15, 15));
		break;
	case KochaEngine::TREE1:
		obj->SetTexture("Resources/Texture/Object/tree1.png");
		obj->SetScale(Vector3(20, 25, 25));
		break;
	case KochaEngine::TREE2:
		obj->SetTexture("Resources/Texture/Object/tree2.png");
		obj->SetScale(Vector3(20, 25, 25));
		break;
	case KochaEngine::GRASS1:
		obj->SetTexture("Resources/Texture/Object/grass1.png");
		obj->SetScale(Vector3(6, 6, 6));
		break;
	case KochaEngine::GRAVE_STONE:
		obj->SetTexture("Resources/Texture/Object/graveStone.png");
		obj->SetScale(Vector3(8, 8, 8));
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
