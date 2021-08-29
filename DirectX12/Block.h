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
		Camera* camera;
		BlockType blockType;

	public:
		Block(Camera* arg_camera, const BlockType arg_blockType, const Vector3& arg_position);
		~Block() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* camera,LightManager* arg_ligtManager) override;
	};
}
