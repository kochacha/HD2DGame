#include "Block.h"

KochaEngine::Block::Block(Camera* arg_camera, const BlockType arg_blockType, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;

	camera = arg_camera;
	blockType = arg_blockType;
	position = arg_position;

	switch (blockType)
	{
	case KochaEngine::SOIL_BLOCK:
		obj = new Object("Ground1");
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
	obj->SetScale(Vector3(3,3,3));
}

void KochaEngine::Block::Update()
{
}

void KochaEngine::Block::ObjDraw(Camera* camera, LightManager* arg_lightManager)
{
	obj->Draw(camera, arg_lightManager);
}
