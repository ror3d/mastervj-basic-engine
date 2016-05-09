#include "ScriptedComponent.h"

#include <Core/Engine/Engine.h>
#include <Base/Scripting/ScriptManager.h>

#include <selene.h>

#include <sstream>
#include<fstream>

unsigned CScriptedComponent::s_nextComponentStateId = 0;

std::vector<CScriptedComponent*> CScriptedComponent::s_components;

void CScriptedComponent::InitAll()
{
	for (auto &comp : s_components)
	{
		comp->Init();
	}
	s_components.clear();
}

CScriptedComponent::CScriptedComponent(const std::string& Name,
                                       CRenderableObject* Owner)
	: CComponent(Name, Owner)
	, m_scriptMgr(CEngine::GetSingleton().getScriptManager())
	, m_componentStateId(s_nextComponentStateId++)
{
	if (m_scriptMgr->IsInitialized())
	{
		Init();
	}
	else
	{
		s_components.push_back(this);
	}
	/*
	std::ifstream t(Name);
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);

	std::stringstream ss;
	ss << "_currentComponent = { " << buffer << " };"
	   << "_componentStates[" << m_componentStateId << "] = _currentComponent;";

	m_scriptMgr->RunCode(ss.str());

	m_scriptMgr->RunCode("_currentComponent.OnCreate()");
	*/

	/*
	std::stringstream ss;
	ss << "_currentComponent = " << Name << ":new();"
		<< "_componentStates[" << m_componentStateId << "] = _currentComponent;"
		<< "_currentComponent:OnCreate();";
	m_scriptMgr->RunCode(ss.str());
	*/
}

CScriptedComponent::~CScriptedComponent()
{}

void CScriptedComponent::Init()
{
	if (m_componentStateId == 0)
	{
		m_scriptMgr->RunCode("_componentStates = {}");
	}

	std::string script = m_scriptMgr->GetScript(getName());
	DEBUG_ASSERT(script != "");

	sel::State* state = m_scriptMgr->getLuaState();

	(*state)["_currentComponent"].SetObj(*this, "GetGameObject", &CComponent::GetOwner);


	std::stringstream ss;
	ss << getName() << " = _currentComponent;"
		<< script
		<< "_componentStates[" << m_componentStateId << "] = _currentComponent;"
		"if (_currentComponent.OnCreate ~= nil) then _currentComponent:OnCreate(); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::Destroy()
{
	std::stringstream ss;
	ss << "_currentComponent = _componentStates[" << m_componentStateId << "];"
		<< "if (_currentComponent.OnDestroy ~= nil) then _currentComponent:OnDestroy(); end"
		<< "_currentComponent = nil;"
		<< "_componentStates[" << m_componentStateId << "] = nil;";
	m_scriptMgr->RunCode(ss.str());
}


void CScriptedComponent::SetComponent()
{
	std::stringstream ss;
	ss << "_currentComponent = _componentStates[" << m_componentStateId << "];";

	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::Update(float ElapsedTime)
{
	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnUpdate ~= nil) then _currentComponent:OnUpdate(" << ElapsedTime << "); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::Render(CContextManager&  _context)
{
	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnRender ~= nil) then _currentComponent:OnRender(); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::RenderDebug(CContextManager&  _context)
{
	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnRenderDebug ~= nil) then _currentComponent:OnRenderDebug(); end";
	m_scriptMgr->RunCode(ss.str());
}

