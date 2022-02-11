#include "SceneChangeBlock.h"

KochaEngine::SceneChangeBlock::SceneChangeBlock(const SceneChangePoint& arg_sceneChangePoint, const Vector3& arg_position, const float arg_size)
{
	position = arg_position;
	size = arg_size;
	sceneChangePoint = arg_sceneChangePoint;
	Initialize();
}

KochaEngine::SceneChangeBlock::~SceneChangeBlock()
{
}

void KochaEngine::SceneChangeBlock::Initialize()
{
	box.position = Vector3(position.x + 1, position.y, position.z);
	box.edgeLength = size; //normalSize = 10
	box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
	box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
}

void KochaEngine::SceneChangeBlock::Update()
{
}

KochaEngine::GameObjectType KochaEngine::SceneChangeBlock::GetType()
{
	return SCENE_CHANGE_BLOCK;
}
