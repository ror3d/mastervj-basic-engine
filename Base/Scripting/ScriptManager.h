#pragma once

#include <string>

namespace sel {
	class State;
}

class CScriptManager
{
public:
	CScriptManager();
	~CScriptManager();

public:
	void Initialize();
	void Destroy();

	void RunCode(const std::string &Code);
	void RunFile(const std::string &FileName);
	void Load(const std::string &XMLFile);

	void RegisterLUAFunctions();
	sel::State * getLuaState(){ return m_state; }

private:
	sel::State *m_state;
};

