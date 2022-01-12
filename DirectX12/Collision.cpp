#include "Collision.h"
#include <cmath>

bool KochaEngine::Collision::HitSphereToSphere(_Sphere sphere1, _Sphere sphere2)
{
	float x = sphere2.position.x - sphere1.position.x;
	float y = sphere2.position.y - sphere1.position.y;
	float z = sphere2.position.z - sphere1.position.z;
	float c = x * x + y * y + z * z;
	float sum_radius = sphere1.radius + sphere2.radius;

	return c < sum_radius * sum_radius;
}

bool KochaEngine::Collision::HitSphereToBox(_Sphere sphere, _Box box)
{
	if (box.rightDownBack.x < (sphere.position.x - sphere.radius))
	{
		return false;
	}
	if (box.leftUpFront.x > (sphere.position.x + sphere.radius))
	{
		return false;
	}
	if (box.rightDownBack.y > (sphere.position.y + sphere.radius))
	{
		return false;
	}
	if (box.leftUpFront.y < (sphere.position.y - sphere.radius))
	{
		return false;
	}
	if (box.rightDownBack.z < (sphere.position.z - sphere.radius))
	{
		return false;
	}
	if (box.leftUpFront.z > (sphere.position.z + sphere.radius))
	{
		return false;
	}
	return true;
}

bool KochaEngine::Collision::HitBoxToBox(_Box box1, _Box box2)
{
	return false;
}

KochaEngine::CollisionFace KochaEngine::Collision::CheckHitFaceX(const _Sphere & sphere, const _Box & box)
{
	Vector3 center = (box.leftUpFront + box.rightDownBack) * 0.5f;
	Vector3 pos = sphere.position;

	Vector3 distance{};
	distance.x = pos.x - center.x;
	distance.z = pos.z - center.z;
	float direction = abs(distance.z / distance.x);

	float tiltXZ = 0;
	Vector3 maxPosition = box.leftUpFront;
	tiltXZ = abs((maxPosition.z - center.z) / (maxPosition.x - center.x));

	if (direction > tiltXZ)
	{
		return NONE;
	}
	else
	{
		if (distance.x > 0)
		{
			return Face_Right;
		}
		else
		{
			return Face_Left;
		}
	}
}

KochaEngine::CollisionFace KochaEngine::Collision::CheckHitFaceZ(const _Sphere & sphere, const _Box & box)
{
	Vector3 center = (box.leftUpFront + box.rightDownBack) * 0.5f;
	Vector3 pos = sphere.position;

	Vector3 distance{};
	distance.x = pos.x - center.x;
	distance.z = pos.z - center.z;
	float direction = abs(distance.z / distance.x);
		
	float tiltXZ = 0;
	Vector3 maxPosition = box.leftUpFront;
	tiltXZ = abs((maxPosition.z - center.z) / (maxPosition.x - center.x));


	if (direction < tiltXZ)
	{
		return NONE;
	}
	else
	{
		if (distance.z > 0)
		{
			return Face_Back;
		}
		else
		{
			return Face_Front;
		}
	}
	
}

bool KochaEngine::Collision::HitCircleToCircle(_Circle circle1, _Circle circle2)
{
	return false;
}

bool KochaEngine::Collision::HitCircleToSquare(_Circle circle, _Square square)
{
	return false;
}

bool KochaEngine::Collision::HitSquareToSquare(_Square square1, _Square square2)
{
	return false;
}

float KochaEngine::Collision::Distance2D(Vector2 pos1, Vector2 pos2)
{
	float distance;
	float dx, dy;

	dx = pos1.x - pos2.x;
	dy = pos1.y - pos2.y;

	distance = (float)sqrt(dx * dx + dy * dy);

	return distance;
}

float KochaEngine::Collision::Distance3D(const Vector3& pos1, const Vector3& pos2)
{
	float distance;
	float dx, dy, dz;

	dx = pos1.x - pos2.x;
	dy = pos1.y - pos2.y;
	dz = pos1.z - pos2.z;

	distance = (float)sqrt(dx * dx + dy * dy + dz * dz);

	return distance;
}

KochaEngine::Vector3 KochaEngine::Collision::GetLeftUpFront(const Vector3& arg_pos, const float arg_edgeLength)
{
	Vector3 result;
	result.x = arg_pos.x - arg_edgeLength * 0.5f;
	result.y = arg_pos.y + arg_edgeLength * 0.5f;
	result.z = arg_pos.z - arg_edgeLength * 0.5f;
	return result;
}

KochaEngine::Vector3 KochaEngine::Collision::GetRightDownBack(const Vector3& arg_pos, const float arg_edgeLength)
{
	Vector3 result;
	result.x = arg_pos.x + arg_edgeLength * 0.5f;
	result.y = arg_pos.y - arg_edgeLength * 0.5f;
	result.z = arg_pos.z + arg_edgeLength * 0.5f;
	return result;
}
