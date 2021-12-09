#include "Number3DOperation.h"
#include "Number3D.h"
#include "Camera.h"
#include "LightManager.h"

KochaEngine::Number3DOperation::Number3DOperation(const int arg_number, const Vector3& arg_position, const Vector3& arg_scale, const int arg_life)
{
	position = arg_position;
	number = new Number3D(arg_number, position, arg_scale);

	life = arg_life;
}

KochaEngine::Number3DOperation::~Number3DOperation()
{
	delete number;
}

void KochaEngine::Number3DOperation::Update()
{
	life--;
	position.y += 0.1f;
	number->SetPosition(position);
}

void KochaEngine::Number3DOperation::Draw(Camera* arg_camera, LightManager* arg_lightManager)
{
	number->Draw(arg_camera, arg_lightManager);
}
