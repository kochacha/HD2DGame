#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	enum FieldAlphaObjType
	{
		ROCK1,
		ROCK2,
		ROCK3,
		TREE1,
		TREE2,
		GRASS1,
	};

	class FieldAlphaObject : public GameObject
	{
	private:
		FieldAlphaObjType fieldAlphaObjType;

	public:
		FieldAlphaObject(Camera* arg_camera, const FieldAlphaObjType& arg_fieldAlphaObjType, const Vector3& arg_position);
		~FieldAlphaObject() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera, LightManager* arg_ligtManager) override;
	};
}