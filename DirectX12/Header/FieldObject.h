#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	enum FieldObjType
	{
		TREE,
		SAKU_WOODEN_SIDEWAYS_UP,
		SAKU_WOODEN_SIDEWAYS_DOWN,
		SAKU_WOODEN_VERTICAL_LEFT,
		SAKU_WOODEN_VERTICAL_RIGHT,
	};

	class FieldObject : public GameObject
	{
	private:
		FieldObjType fieldObjType;

	public:
		FieldObject(Camera* arg_camera, const FieldObjType arg_fieldObjType, const Vector3& arg_position);
		~FieldObject() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera, LightManager* arg_ligtManager) override;
	};
}


