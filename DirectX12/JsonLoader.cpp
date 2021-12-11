#include "JsonLoader.h"
#include <fstream>
#include <sstream>

std::string KochaEngine::JsonLoader::ReadTxt(const std::string& arg_path)
{
	std::ifstream ifs(arg_path);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	std::string str = buffer.str();
	if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)  return str.substr(3);
	else  return str;
}

picojson::object KochaEngine::JsonLoader::ReadJsonObj(const std::string& arg_path)
{
	std::ifstream ifs(arg_path);
	if (!ifs)
	{
		throw std::exception("File cannot be opened.");
	}

	std::string json = ReadTxt(arg_path);
	picojson::value val;
	std::string err;
	picojson::parse(val, json.c_str(), json.c_str() + strlen(json.c_str()), &err);
	if (!err.empty())
	{
		throw std::exception("JSON format error in");
	}
	return val.get<picojson::object>();
}
