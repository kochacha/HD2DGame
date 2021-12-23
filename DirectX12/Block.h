#pragma once
#include "GameObject.h"
#include "Camera.h"


namespace KochaEngine
{
	enum BlockType
	{
		SOIL_BLOCK,
	};

	class Block : public GameObject
	{

	private:
		BlockType blockType;

	public:
		Block(const BlockType arg_blockType, const Vector3& arg_position);
		~Block() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera,LightManager* arg_ligtManager) override;
	};
}
