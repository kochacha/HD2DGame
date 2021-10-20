#pragma once
#include "Object.h"
#include "ParticleParam.h"

namespace KochaEngine
{
	class Camera;
	class LightManager;

	class Particle
	{
	private:
		Object* obj;
		ParticleParam param;
		bool isDead;

	public:
		Particle(const ParticleParam& arg_param);
		~Particle();

		void Update();
		void Draw(Camera* arg_camera, LightManager* arg_lightManager);

		inline const bool IsDead() { return isDead; }
	};
}