#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>

/*
 * This class captures errors printed by lua to redirect them as we wish.
 */
class LuaErrorCapturedStdout
{
public:
	LuaErrorCapturedStdout()
	{
		_old = std::cout.rdbuf(_out.rdbuf());
	}

	~LuaErrorCapturedStdout()
	{
		std::cout.rdbuf(_old);
		OutputDebugStringA(_out.str().c_str());
	}

	inline std::string Content() const
	{
		return _out.str();
	}

private:
	std::stringstream _out;
	std::streambuf* _old;
};

