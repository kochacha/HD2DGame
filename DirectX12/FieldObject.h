#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	enum FieldObjType
	{
		TREE,
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


