
#pragma once

#include <string>
#include <vector>
#include <map>

namespace ApiDocs
{
	struct Parameter
	{
		std::string Name;
		std::string Type;
	};

	struct ReturnValue
	{
		std::string Type;
	};

	struct Overload
	{
		std::vector<Parameter> Params;
		std::vector<ReturnValue> Returns;
		bool IsStatic;
		std::string Desc;
		std::string Notes;
	};

	struct Function
	{
		std::string Name;
		std::vector<Overload> Overloads;
	};

	struct Constant
	{
		std::string Name;
		std::string Value;
		std::string Desc;
	};

	struct Variable
	{
		std::string Name;
		std::string Type;
		std::string Desc;
	};

	struct EnumValue
	{
		std::string Name;
		std::string Value;
	};

	struct Enum
	{
		std::string Name;
		std::vector<EnumValue> Values;
	};

	struct Class
	{
		std::string Name;
		std::string Desc;
		std::vector<std::string> Inherits;
		std::map<std::string, Function> Functions;
		std::map<std::string, Constant> Constants;
		std::map<std::string, Variable> Variables;
		std::map<std::string, Enum> Enums;
	};

	/** Returns the complete API registry. */
	const std::map<std::string, Class> & GetApiRegistry();
}
