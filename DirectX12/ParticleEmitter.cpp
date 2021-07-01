#include "ParticleEmitter.h"
#include "Util.h"

KochaEngine::ParticleEmitter::ParticleEmitter(ParticleManager* pManager)
{
	this->pManager = pManager;
}

KochaEngine::ParticleEmitter::~ParticleEmitter()
{
}

void KochaEngine::ParticleEmitter::HitParticle(Vector3 position)
{
	ParticleParam param;
	param.position = position;

	for (int i = 0; i < MAX_EXPROTION; i++)
	{
		if (i % 2)
		{
			param.color = { 0.9f,0.7f,0,1 };
		}
		else
		{
			param.color = { 0.7f,0.5f,0.7f,1 };
		}

		param.life = 20;
		param.moveColor = { 0,0,0,-0.05f };
		param.moveRotate = { 3,3,0 };
		param.moveScale = { 0,0,0 };
		float rand = ((float)Util::GetRandInt(10) + 30) / 3.0f;
		param.scale = { rand, rand ,rand };
		param.velocity = { ((float)Util::GetRandInt(30) - 15) / 10, ((float)Util::GetRandInt(15)) / 10,((float)Util::GetRandInt(30) - 15) / 10 };

		pManager->AddParticle(new Particle(param));
	}
}
