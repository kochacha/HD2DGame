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

		void LoadCSV(const int arg_num, const std::string& arg_fileName);
		std::vector<std::vector<int>> GetMapData(const int arg_num);
		void Clear();
	};
}
