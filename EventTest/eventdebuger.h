#pragma once

#include <sstream>
#include <string>

class EventDebugger
{
public:
	template<typename T>
	static std::string inspect(const T &e)
	{
		std::stringstream ss;
		ss << "'" << e.dbg_function << "' in '" << e.dbg_file << ":(" << e.dbg_line << ")";
		return ss.str();
	}	
};