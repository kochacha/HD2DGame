#include "ParticleManager.h"

void KochaEngine::ParticleManager::Zsort()
{
}

KochaEngine::ParticleManager::ParticleManager()
{
	RemoveAll();
}

KochaEngine::ParticleManager::~ParticleManager()
{
	RemoveAll();
}

void KochaEngine::ParticleManager::AddParticle(Particle* arg_particle)
{
	particles.push_back(arg_particle);
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

void KochaEngine::ParticleManager::Draw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	auto end = particles.end();
	for (auto it = particles.begin(); it != end; ++it)
	{
		(*it)->Draw(arg_camera, arg_lightManager);
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
