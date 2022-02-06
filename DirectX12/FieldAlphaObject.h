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
		WITHER_TREE1,
		GRASS1,
		GRAVE_STONE,
	};

	class FieldAlphaObject : public GameObject
	{
	private:
		FieldAlphaObjType fieldAlphaObjType;
		int preEnvironmentNum;

		void UpdateEnvironment();

	public:
		FieldAlphaObject(const FieldAlphaObjType& arg_fieldAlphaObjType, const Vector3& arg_position);
		~FieldAlphaObject() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera, LightManager* arg_ligtManager) override;
	};
}