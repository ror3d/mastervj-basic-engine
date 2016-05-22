#include "ScriptedComponent.h"

#include <Core/Engine/Engine.h>
#include <Base/Scripting/ScriptManager.h>
#include <Graphics/Renderable/RenderableObject.h>

#include <selene.h>

#include <sstream>
#include <fstream>

unsigned CScriptedComponent::s_nextComponentStateId = 0;

CScriptedComponent::CScriptedComponent(const std::string& name,
									   CRenderableObject* Owner)
	: CComponent(name, Owner)
	, m_scriptMgr(CEngine::GetSingleton().getScriptManager())
	, m_componentStateId(s_nextComponentStateId++)
{
}

CScriptedComponent::CScriptedComponent(CXMLTreeNode& node,
									   CRenderableObject* Owner)
	: CComponent(node, Owner)
	, m_scriptMgr(CEngine::GetSingleton().getScriptManager())
	, m_componentStateId(s_nextComponentStateId++)
{
	std::string name = node.GetPszProperty("class");
	setName(name);
}

CScriptedComponent::~CScriptedComponent()
{}

void CScriptedComponent::Init()
{
	m_scriptMgr->RunCode( "if (_componentStates == nil) then _componentStates = {}; end" );

	std::string script = m_scriptMgr->GetScript(getName());
	DEBUG_ASSERT(script != "");

	sel::State* state = m_scriptMgr->getLuaState();

	(*state)["_currentComponent"]
		.SetObj(*this,
			"GetGameObject", &CComponent::GetOwner,
			"SendMessage", &CComponent::SendMsg,
			"SetEnabled", &CComponent::SetEnabled,
			"GetEnabled", &CComponent::GetEnabled);


	std::stringstream ss;
	ss << getName() << " = _currentComponent;"
		<< script
		<< "_componentStates[" << m_componentStateId << "] = _currentComponent;";
	for (auto const& prop : m_properties)
	{
		std::string val = prop.value;
		if (prop.type == "string")
		{
			val = "\"" + val + "\"";
		}
		ss << getName() << "." << prop.name << " = " << val << ";";
	}
	ss << "if (_currentComponent.OnCreate ~= nil) then _currentComponent:OnCreate(); end\n";
	//OutputDebugStringA(ss.str().c_str());
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::OnObjectInitialized()
{
	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnObjectInitialized ~= nil) then _currentComponent:OnObjectInitialized(); end";
	m_scriptMgr->RunCode( ss.str() );
}

void CScriptedComponent::Destroy()
{
	std::stringstream ss;
	ss << "_currentComponent = _componentStates[" << m_componentStateId << "];"
		<< "if (_currentComponent.OnDestroy ~= nil) then _currentComponent:OnDestroy(); end\n"
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
	if ( ! GetOwner()->GetVisible() )
	{
		return;
	}

	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnUpdate ~= nil) then _currentComponent:OnUpdate(" << ElapsedTime << "); end";
	m_scriptMgr->RunCode( ss.str() );
}

void CScriptedComponent::FixedUpdate(float ElapsedTime)
{
	if ( ! GetOwner()->GetVisible() )
	{
		return;
	}

	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnFixedUpdate ~= nil) then _currentComponent:OnFixedUpdate(" << ElapsedTime << "); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::Render(CContextManager&  _context)
{
	if ( ! GetOwner()->GetVisible() )
	{
		return;
	}

	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnRender ~= nil) then _currentComponent:OnRender(); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::RenderDebug(CContextManager&  _context)
{
	if ( ! GetOwner()->GetVisible() )
	{
		return;
	}

	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent.OnRenderDebug ~= nil) then _currentComponent:OnRenderDebug(); end";
	m_scriptMgr->RunCode(ss.str());
}

void CScriptedComponent::SendMsg(const std::string msg)
{
	SetComponent();

	std::stringstream ss;
	ss << "if (_currentComponent." << msg << " ~= nil) then _currentComponent:" << msg << "(); end";
	m_scriptMgr->RunCode(ss.str());
}
