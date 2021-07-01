#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	struct ParticleParam
	{
		Vector3 position;
		Vector3 velocity;
		Vector3 scale;
		Vector3 moveScale;
		Vector3 rotate;
		Vector3 moveRotate;
		Vector4 color;
		Vector4 moveColor;
		int life;
	};
}