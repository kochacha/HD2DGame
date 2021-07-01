#include "ParticleManager.h"

KochaEngine::ParticleManager::ParticleManager()
{
	RemoveAll();
}

KochaEngine::ParticleManager::~ParticleManager()
{
	RemoveAll();
}

void KochaEngine::ParticleManager::AddParticle(Particle * particle)
{
	particles.push_back(particle);
}

void KochaEngine::ParticleManager::Update()
{
	auto end = particles.end();
	for (auto it = particles.begin(); it != end; ++it)
	{
		(*it)->Update();
	}
	Remove();
}

void KochaEngine::ParticleManager::Draw(Camera* camera)
{
	if (camera == nullptr) return;
	auto end = particles.end();
	for (auto it = particles.begin(); it != end; ++it)
	{
		(*it)->Draw(camera);
	}
}

void KochaEngine::ParticleManager::RemoveAll()
{
	auto end = particles.end();
	for (auto it = particles.begin(); it != end; ++it)
	{
		delete *it;
	}
	particles.clear();
}

void KochaEngine::ParticleManager::Remove()
{
	for (auto it = particles.begin(); it != particles.end();)
	{
		if (!(*it)->IsDead())
		{
			++it;
			continue;
		}
		delete *it;
		it = particles.erase(it);
	}
}
