#pragma once
#include "Particle.h"
#include "ParticleManager.h"

namespace KochaEngine
{
	enum
	{
		MAX_BAKUGEKI = 4,
		MAX_HITPARTICLE = 4,
		MAX_EXPROTION = 8,
		MAX_SHOT = 6,
		MAX_CHIT = 8,
	};

	class ParticleEmitter
	{
	private:
		ParticleManager* pManager;

	public:
		ParticleEmitter(ParticleManager* pManager);
		~ParticleEmitter();

		void HitParticle(Vector3 position);
	};
}
