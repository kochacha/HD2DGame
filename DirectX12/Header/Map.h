#pragma once
#include "GameObjectManager.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "CSVReader.h"

namespace KochaEngine
{
	class Map
	{
	private:
		const float CHIP_SIZE = 16;
		std::vector<std::vector<int>> mapData;

		GameObjectManager* gManager;
		Camera* camera;
		CSVReader csvReader;

	public:
		Map(GameObjectManager* arg_gManager, Camera* arg_camera);
		~Map();

		void CreateMap(const int arg_mapNum);
	};
}
