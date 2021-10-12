#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace KochaEngine
{
	class CSVReader
	{
	public:
		CSVReader();
		~CSVReader();

		std::map<int, std::vector<std::vector<int>>> mapList;

		void LoadCSV(int num, std::string fileName);
		std::vector<std::vector<int>> GetMapData(int num);
		void Clear();
	};
}
