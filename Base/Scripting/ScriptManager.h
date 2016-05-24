#pragma once

#include <string>
#include <map>

namespace sel {
	class State;
}

class CScriptManager
{
public:
	CScriptManager();
	~CScriptManager();

public:
	void Initialize(const std::string& file);
	bool IsInitialized() { return m_initialized; }
	void destroy() {}

	void RunCode(const std::string &Code);

	void RunFile(const std::string &FileName);
	void Load(const std::string &XMLFile);

	void RegisterLUAFunctions();
	sel::State * getLuaState(){ return m_state; }

	const std::string &GetScript(const std::string &name)
	{
		auto it = m_loadedScripts.find(name);
		if (it != m_loadedScripts.end())
		{
			return it->second;
		}
		return "";
	}
private:
	sel::State *m_state;

	std::map<std::string, std::string> m_loadedScripts;

	bool m_initialized;
};

