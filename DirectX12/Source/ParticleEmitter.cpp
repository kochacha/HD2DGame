#include "../Header/ParticleEmitter.h"
#include "../Header/Util.h"

KochaEngine::ParticleEmitter::ParticleEmitter(ParticleManager* pManager)
{
	this->pManager = pManager;
}

KochaEngine::ParticleEmitter::~ParticleEmitter()
{
}

void KochaEngine::ParticleEmitter::MoveParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	param.life = 20;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(-0.2f, -0.2f, 0);
	param.scale = Vector3(4, 4, 1);
	param.textureName = "Resources/white.png";
	param.velocity = Vector3(0, 0, 0);
	param.color = Vector4(0, 0, 0, 1);
	//auto rand = Util::GetIntRand(0, 360);
	param.rotate = Vector3(0, 0, 45);

	pManager->AddParticle(new Particle(param));
}

void KochaEngine::ParticleEmitter::SmashStar(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	auto randScale = (float)Util::GetIntRand(5, 8);
	auto randY = (float)(Util::GetIntRand(0, 50) - 25) * 0.1f;
	param.life = 50;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, randY);
	param.moveScale = Vector3(-randScale / (float)param.life, -randScale / (float)param.life, 0);
	param.scale = Vector3(randScale, randScale, 1);
	param.textureName = "Resources/white.png";
	param.velocity = Vector3(0, randY, 0);
	param.color = Vector4(0, 0, 0, 1);
	param.rotate = Vector3(0, 0, 0);

	pManager->AddParticle(new Particle(param));

}