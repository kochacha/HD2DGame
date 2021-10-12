#include "../Header/CollisionBlock.h"

KochaEngine::CollisionBlock::CollisionBlock(const Vector3& arg_position, const float arg_size)
{
	position = arg_position;
	size = arg_size;
	Initialize();
}

KochaEngine::CollisionBlock::~CollisionBlock()
{
}

void KochaEngine::CollisionBlock::Initialize()
{
	box.position = Vector3(position.x + 1, position.y, position.z);
	box.edgeLength = size; //normalSize = 10
	box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
	box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
}

void KochaEngine::CollisionBlock::Update()
{
}

KochaEngine::GameObjectType KochaEngine::CollisionBlock::GetType()
{
	return COLLISION_BLOCK;
}
