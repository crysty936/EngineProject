#pragma once

#include <string>

namespace Engine
{
	class ParserObj
	{
	public:
		static class Model* ParseGetModel(std::string Path);
	};
}