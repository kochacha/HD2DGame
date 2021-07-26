#include "GameObject.h"


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

void KochaEngine::GameObject::ObjDraw(Camera* camera)
{
}

void KochaEngine::GameObject::SpriteDraw()
{
}

void KochaEngine::GameObject::Hit()
{
}

void KochaEngine::GameObject::HitBlock(_Square arg_square)
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
