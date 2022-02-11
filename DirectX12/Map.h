#pragma once
#include "GameObjectManager.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "CSVReader.h"
#include <memory>

namespace KochaEngine
{
	class Map
	{
	private:
		const float CHIP_SIZE = 16;
		std::vector<std::vector<int>> mapData;

		std::weak_ptr<GameObjectManager> gManager;
		std::weak_ptr<Camera> camera;
		CSVReader csvReader;

	public:
		Map(std::weak_ptr<GameObjectManager> arg_gManager, std::weak_ptr<Camera> arg_camera);
		~Map();

		void CreateMap(const int arg_mapNum);
	};
}
