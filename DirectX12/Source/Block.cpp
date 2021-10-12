#include "../Header/Block.h"

KochaEngine::Block::Block(const BlockType arg_blockType, const Vector3& arg_position)
{
	blockType = arg_blockType;
	position = arg_position;

	switch (blockType)
	{
	case KochaEngine::SOIL_BLOCK:
		obj = new Object("box");
		//obj->SetTexture("Resources/PIEN.png");
		break;
	default:
		obj = new Object("box");
		break;
	}
	Initialize();
}

KochaEngine::Block::~Block()
{
	delete obj;
}

void KochaEngine::Block::Initialize()
{
	obj->SetPosition(position);
	obj->SetScale(Vector3(8,8,8));

	isAlpha = true;
}

void KochaEngine::Block::Update()
{
	obj->MoveRotate(Vector3(-0.1f,0,0));
}

void KochaEngine::Block::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}
