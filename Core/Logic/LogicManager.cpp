#include "LogicManager.h"

#include <Core/Engine/Engine.h>
#include <Base/Scripting/ScriptManager.h>

CLogicManager::CLogicManager()
	: m_FileName("")
{
}

CLogicManager::~CLogicManager()
{
	m_activedLogic = new std::vector<std::string>();
}

void CLogicManager::Update()
{
	CEngine::GetSingleton().getScriptManager()->RunCode("CinematicsActionManager_Update()");
}

void CLogicManager::LoadXML(std::string fileName)
{
	m_FileName = fileName;
	CEngine::GetSingleton().getScriptManager()->RunCode("LogicManager_LoadXML()");
}

std::string CLogicManager::GetActivableObject()
{
	if (m_activedLogic->size() > 0)
	{
		return m_activedLogic->back();
	}
	else
	{
		return "";
	}
	
}

void CLogicManager::RemoveActivedLogic(std::string nameToRemove)
{
	for (int i = 0; i < m_activedLogic->size(); i++){
		if (m_activedLogic->at(i) == nameToRemove){
			m_activedLogic->erase(m_activedLogic->begin()+i);
			break;
		}
	}
}
