#include "Number3DEmitter.h"
#include "Number3DManager.h"
#include "Number3DOperation.h"

KochaEngine::Number3DEmitter::Number3DEmitter(Number3DManager* arg_n3DManager)
{
	if (arg_n3DManager == nullptr) return;
	n3DManager = arg_n3DManager;
}

KochaEngine::Number3DEmitter::~Number3DEmitter()
{
}

void KochaEngine::Number3DEmitter::AddNumber3D(const Vector3& arg_position, const int arg_number)
{
	n3DManager->AddNumbers(new Number3DOperation(arg_number, arg_position, Vector3(2.5, 2.5, 2.5), 70));
}
