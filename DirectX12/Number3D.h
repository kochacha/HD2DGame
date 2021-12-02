#pragma once

namespace KochaEngine
{
	class Camera;
	class LightManager;

	class Number3D
	{
	private:


	public:
		Number3D();
		~Number3D();


		void Draw(Camera* arg_camera, LightManager* arg_ligtManager);
	};
}