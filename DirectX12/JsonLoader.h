#pragma once
#include "picojson.h"

namespace KochaEngine
{
	class JsonLoader
	{
	private:

		static std::string ReadTxt(const std::string& arg_path);
	public:

		static picojson::object ReadJsonObj(const std::string& arg_path);
	};
}