#pragma once
#include <vector>
#include "Vector3.h"

namespace KochaEngine
{
	class Number3DOperation;
	class Camera;
	class LightManager;

	class Number3DManager
	{
	private:
		std::vector<Number3DOperation*> numbers;

	public:
		Number3DManager();
		~Number3DManager();

		void AddNumbers(Number3DOperation* arg_number3D);

		void Update();
		void Draw(Camera* arg_camera, LightManager* arg_lightManager);

		void Remove();
		void RemoveAll();
	};
}