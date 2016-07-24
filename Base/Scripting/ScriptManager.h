#pragma once

#include <string>
#include <map>
#include <set>

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
	void destroy();

	void RunCode(const std::string &Code);
	void RunCodeIf(const std::string &condition, const std::string &Code);

	void RunFile(const std::string &FileName);
	void Load(const std::string &XMLFile);

	void RegisterLUAFunctions();
	sel::State * getLuaState(){ return m_state; }

	void RunScript(const std::string &name);

	inline const std::string &GetScript(const std::string &name) const
	{
		auto it = m_loadedScripts.find(name);
		if (it != m_loadedScripts.end())
		{
			return it->second;
		}
		return "";
	}

	inline void AddScriptReferenceComponent( const std::string &scriptName, const std::string &componentName )
	{
		m_scriptReferences[scriptName].emplace( componentName );
	}

	inline void RemoveScriptReferenceComponent( const std::string &scriptName, const std::string &componentName )
	{
		m_scriptReferences[scriptName].erase( componentName );
	}

	bool ReloadScript( const std::string &scriptName );

	inline std::set<std::string> GetLoadedScriptNames() const
	{
		std::set<std::string> ret;
		for ( auto &const p : m_loadedScripts )
		{
			ret.emplace( p.first );
		}
		return ret;
	}

private:
	sel::State *m_state;

	std::map<std::string, std::string> m_scriptFiles;

	std::map<std::string, std::string> m_loadedScripts;

	std::map<std::string, std::set<std::string>> m_scriptReferences;

	bool m_initialized;
};

