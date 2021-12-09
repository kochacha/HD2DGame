#pragma once
#include "Vector3.h"

namespace KochaEngine
{
	class Number3DManager;

	class Number3DEmitter
	{
	private:
		Number3DManager* n3DManager;

	public:
		Number3DEmitter(Number3DManager* arg_n3DManager);
		~Number3DEmitter();

		void AddNumber3D(const Vector3& arg_position, const int arg_number);

	};
}