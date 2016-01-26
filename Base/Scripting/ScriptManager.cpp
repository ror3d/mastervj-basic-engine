#include "ScriptManager.h"

#include <selene.h>


CScriptManager::CScriptManager()
	: m_state(nullptr)
{
}


CScriptManager::~CScriptManager()
{
	if (m_state)
	{
		delete m_state;
	}
}

void CScriptManager::Initialize()
{
	m_state = new sel::State{ true };
}

void CScriptManager::Destroy()
{
}

void CScriptManager::RunCode(const std::string &Code) const
{

}

void CScriptManager::RunFile(const std::string &FileName) const
{
}

void CScriptManager::Load(const std::string &XMLFile)
{
}

void CScriptManager::RegisterLUAFunctions()
{
}
