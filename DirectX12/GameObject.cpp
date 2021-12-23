#include "GameObject.h"
#include "Collision.h"

KochaEngine::GameObject::GameObject()
{
}

KochaEngine::GameObject::~GameObject()
{
}

void KochaEngine::GameObject::Initialize()
{
}

void KochaEngine::GameObject::Update()
{
}

void KochaEngine::GameObject::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
}

void KochaEngine::GameObject::SpriteDraw()
{
}

void KochaEngine::GameObject::Hit()
{
}

void KochaEngine::GameObject::HitBlock(_Box arg_box)
{
}

void KochaEngine::GameObject::Dead()
{
}

KochaEngine::GameObjectType KochaEngine::GameObject::GetType()
{
	return GameObjectType();
}

float KochaEngine::GameObject::GetSpeed()
{
	return 0.0f;
}

float KochaEngine::GameObject::GetCameraDistance(Camera* arg_camera)
{
	return 0.0f;
}
