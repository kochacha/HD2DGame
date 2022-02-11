#include "SceneSpawnBlock.h"

KochaEngine::SceneSpawnBlock::SceneSpawnBlock(const SceneChangePoint& arg_sceneChangePoint, const Vector3& arg_position, const float arg_size)
{
	position = arg_position;
	size = arg_size;
	sceneChangePoint = arg_sceneChangePoint;
	Initialize();
}

KochaEngine::SceneSpawnBlock::~SceneSpawnBlock()
{
}

void KochaEngine::SceneSpawnBlock::Initialize()
{
	box.position = Vector3(position.x + 1, position.y, position.z);
	box.edgeLength = size; //normalSize = 10
	box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
	box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
}

void KochaEngine::SceneSpawnBlock::Update()
{
}

KochaEngine::GameObjectType KochaEngine::SceneSpawnBlock::GetType()
{
	return SCENE_SPAWN_BLOCK;
}
