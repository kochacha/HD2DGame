#pragma once
#include "Vector3.h"

namespace KochaEngine
{
	class Number3D;
	class Camera;
	class LightManager;

	class Number3DOperation
	{
	private:
		Number3D* number;

		Vector3 position;
		int life;

	public:
		Number3DOperation(const int arg_number, const Vector3& arg_position, const Vector3& arg_scale, const int arg_life);
		~Number3DOperation();

		void Update();
		void Draw(Camera* arg_camera, LightManager* arg_lightManager);

		inline const int GetLife() { return life; }
	};
}