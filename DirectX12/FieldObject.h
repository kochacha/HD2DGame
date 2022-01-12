#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	enum FieldObjType
	{
		TREE,
		SAKU_WOODEN_VERTICAL,
		SAKU_WOODEN_SIDEWAYS,
		WALL_SAND_VERTICAL,
		WALL_SAND_SIDEWAYS,
	};

	class FieldObject : public GameObject
	{
	private:
		FieldObjType fieldObjType;

	public:
		FieldObject(const FieldObjType arg_fieldObjType, const Vector3& arg_position);
		~FieldObject() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_ligtManager) override;
	};
}


