#pragma once
#include <vector>
#include "Particle.h"

namespace KochaEngine
{
	class Camera;
	class LightManager;

	class ParticleManager
	{
	private:
		std::vector<Particle*> particles;

		void Zsort();

	public:
		ParticleManager();
		~ParticleManager();

		void AddParticle(Particle* arg_particle);

		void Update();
		void Draw(Camera* arg_camera, LightManager* arg_lightManager);

		void RemoveAll();
		void Remove();
	};
}
