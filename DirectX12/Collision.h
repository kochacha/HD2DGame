#pragma once
#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"

namespace KochaEngine
{
	//“–‚½‚è”»’è::lŠp
	struct _Square
	{
		Vector2 leftUp;
		Vector2 rightDown;
	};

	//“–‚½‚è”»’è::’¼•û‘Ì
	struct _Box
	{
		Vector3 position;
		float edgeLength;
		Vector3 leftUpFront;
		Vector3 rightDownBack;
	};


	//“–‚½‚è”»’è::‰~
	struct _Circle
	{
		Vector2 position;
		float radius;
	};

	//“–‚½‚è”»’è::‹…
	struct _Sphere
	{
		Vector3 position;
		float radius;
	};

	enum CollisionFace
	{
		Face_Right,
		Face_Left,
		Face_Top,
		Face_Bottom,
		Face_Back,
		Face_Front,
		NONE,
	};

	class Collision
	{
	public:
		//3DÕ“Ë”»’è
		static bool HitSphereToSphere(_Sphere sphere1, _Sphere sphere2);  //‹…    ~‹…
		static bool HitSphereToBox(_Sphere sphere, _Box box);             //‹…    ~’¼•û‘Ì
		static bool HitBoxToBox(_Box box1, _Box box2);                    //’¼•û‘Ì~’¼•û‘Ì

		static CollisionFace CheckHitFaceX(const _Sphere& sphere, const _Box& aabb);
		static CollisionFace CheckHitFaceZ(const _Sphere& sphere, const _Box& aabb);

		//2DÕ“Ë”»’è
		static bool HitCircleToCircle(_Circle circle1, _Circle circle2);  //‰~    ~‰~
		static bool HitCircleToSquare(_Circle circle, _Square square);    //‰~    ~lŠp
		static bool HitSquareToSquare(_Square square1, _Square square2);  //lŠp  ~lŠp

		static float Distance2D(Vector2 pos1, Vector2 pos2);
		static float Distance3D(const Vector3& pos1, const Vector3& pos2);

		static Vector3 GetLeftUpFront(const Vector3& arg_pos, const float arg_edgeLength);
		static Vector3 GetRightDownBack(const Vector3& arg_pos, const float arg_edgeLength);
	};
}