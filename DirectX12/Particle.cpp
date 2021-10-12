#include "Particle.h"
#include "Camera.h"
#include "LightManager.h"

KochaEngine::Particle::Particle(ParticleParam param) : param(param)
{
	obj = new Object("plane");
	obj->SetBillboardType(Object::BILLBOARD);
	obj->SetPosition(param.position);
	obj->SetScale(param.scale);
	obj->SetRotate(param.rotate);
	obj->SetTexture(param.textureName);
	obj->SetColor(param.color);
	isDead = false;
}

KochaEngine::Particle::~Particle()
{
	delete obj;
}

void KochaEngine::Particle::Update()
{
	if (param.life > 0)
	{
		param.life--;
	}
	else
	{
		isDead = true;
		return;
	}
	Vector3 objScale = obj->GetScale();
	if (objScale.x <= 0 && 
		objScale.y <= 0 && 
		objScale.z <= 0)
	{
		isDead = true;
		return;
	}

	obj->MovePosition(param.velocity);
	obj->MoveScale(param.moveScale);
	obj->MoveRotate(param.moveRotate);
	obj->MoveColor(param.moveColor);
}

void KochaEngine::Particle::Draw(Camera* camera, LightManager* arg_lightManager)
{
	if (camera == nullptr) return;
	if (arg_lightManager == nullptr) return;
	obj->Draw(camera, arg_lightManager);
}
