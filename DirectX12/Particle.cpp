#include "Particle.h"
#include "Camera.h"
#include "LightManager.h"

KochaEngine::Particle::Particle(const ParticleParam& arg_param) : param(arg_param)
{
	obj = new Object("plane");
	obj->SetBillboardType(Object::BILLBOARD_Y);
	obj->SetPosition(arg_param.position);
	obj->SetScale(arg_param.scale);
	obj->SetRotate(arg_param.rotate);
	obj->SetTexture(arg_param.textureName);
	obj->SetColor(arg_param.color);
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

void KochaEngine::Particle::Draw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}
