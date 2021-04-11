#pragma once

#include <string>
#include "rapidjson/document.h"

namespace Engine
{
	class Model;

	class ParserObj
	{
		enum class MemberType
		{
			String,
			Bool,
			Float,
			Double,
			UInt
		};
	public:
		static std::unique_ptr<Model> ParseGetModel(std::string Path);
		static void GetMember(rapidjson::Value::ConstObject& Container, const char* MemberName, MemberType Type, OUT void* MemberAdress);
	};
}