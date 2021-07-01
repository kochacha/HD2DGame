#pragma once
#include "Object.h"
#include "ParticleParam.h"

namespace KochaEngine
{
	class Camera;
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
		void Draw(Camera* camera);

		bool IsDead() { return isDead; }
	};
}