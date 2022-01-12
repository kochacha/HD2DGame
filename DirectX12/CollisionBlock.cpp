#include "CollisionBlock.h"

KochaEngine::CollisionBlock::CollisionBlock(const Vector3& arg_position, const float arg_size, const int arg_collisionType)
{
	position = arg_position;
	size = arg_size;
	collisionType = arg_collisionType;
	Initialize();
}

KochaEngine::CollisionBlock::~CollisionBlock()
{
}

void KochaEngine::CollisionBlock::Initialize()
{
	Vector3 _position;
	switch (collisionType)
	{
	case COLLISION_DEFAULT:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
		break;
	case COLLISION_UP:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		_position = Vector3(box.position.x, box.position.y, box.position.z + box.edgeLength * 0.5f);
		box.leftUpFront = Collision::GetLeftUpFront(_position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
		break;
	case COLLISION_DOWN:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		_position = Vector3(box.position.x, box.position.y, box.position.z - box.edgeLength * 0.5f);
		box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(_position, box.edgeLength);
		break;
	case COLLISION_RIGHT:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		_position = Vector3(box.position.x + box.edgeLength * 0.5f, box.position.y, box.position.z);
		box.leftUpFront = Collision::GetLeftUpFront(_position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
		break;
	case COLLISION_LEFT:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		_position = Vector3(box.position.x - box.edgeLength * 0.5f, box.position.y, box.position.z);
		box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(_position, box.edgeLength);
		break;
	default:
		box.position = Vector3(position.x + 1, position.y, position.z);
		box.edgeLength = size; //normalSize = 10
		box.leftUpFront = Collision::GetLeftUpFront(box.position, box.edgeLength);
		box.rightDownBack = Collision::GetRightDownBack(box.position, box.edgeLength);
		break;
	}
}

void KochaEngine::CollisionBlock::Update()
{
}

KochaEngine::GameObjectType KochaEngine::CollisionBlock::GetType()
{
	return COLLISION_BLOCK;
}
