#pragma once
#include "Vector3.h"

namespace KochaEngine
{
#define NUM3D_MAX_DIGIT 7

	class Camera;
	class LightManager;
	class Object;

	class Number3D
	{
	private:
		Object* planes[NUM3D_MAX_DIGIT];

		Vector3 position;
		Vector3 scale;
		unsigned int digit;
		int preNumber;

		void Remove();
		void SetNumber(const int arg_number);
		void FixPosition();

	public:

		Number3D(const Vector3& arg_position, const Vector3& arg_scale);
		Number3D(const int arg_num, const Vector3& arg_position, const Vector3& arg_scale);
		~Number3D();
		
		void Initialize();
		void Draw(Camera* arg_camera, LightManager* arg_ligtManager);
		void Draw(const int arg_number, Camera* arg_camera, LightManager* arg_ligtManager);

		inline void SetPosition(const Vector3& arg_position) { position = arg_position; }
	};
}