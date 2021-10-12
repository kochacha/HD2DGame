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
		Particle(ParticleParam param);
		~Particle();

		void Update();
		void Draw(Camera* camera, LightManager* arg_lightManager);

		bool IsDead() { return isDead; }
	};
}