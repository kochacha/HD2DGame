#include "CSVReader.h"
#include <windows.h>

using namespace std;

KochaEngine::CSVReader::CSVReader()
{
	Clear();
}

KochaEngine::CSVReader::~CSVReader()
{
}

void KochaEngine::CSVReader::LoadCSV(int num, std::string fileName)
{
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		OutputDebugStringA("CSVファイルを開けませんでした");
		return;
	}

	std::vector<std::vector<int>> m_map;
	std::vector<int> row;
	std::string line;

	while (getline(ifs, line))
	{
		std::string work;
		char token = ',';
		std::istringstream stream(line);

		while (getline(stream, work, token))
		{
			row.push_back(std::stoi(work));
		}
		m_map.push_back(row);
		{
			std::vector<int> e;
			e.swap(row);
			row.resize(0);
		}
	}

	mapList.emplace(num, m_map);
}

std::vector<std::vector<int>> KochaEngine::CSVReader::GetMapData(int num)
{
	return this->mapList[num];
}

void KochaEngine::CSVReader::Clear()
{
	if(!mapList.empty())
	{
		std::map<int, std::vector<std::vector<int>>> e;
		e.swap(mapList);
	}
}
