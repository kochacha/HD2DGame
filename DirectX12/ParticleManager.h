#pragma once
#include <vector>
#include "Particle.h"

namespace KochaEngine
{
	class Camera;
	class ParticleManager
	{
	private:
		std::vector<Particle*> particles;

	public:
		ParticleManager();
		~ParticleManager();

		void AddParticle(Particle* particle);

		void Update();
		void Draw(Camera* camera);

		void RemoveAll();
		void Remove();
	};
}
