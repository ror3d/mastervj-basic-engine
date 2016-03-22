#include "ScriptedComponent.h"

#include <Core/Engine/Engine.h>
#include <Base/Scripting/ScriptManager.h>

CScriptedComponent::CScriptedComponent(const std::string &Name, CRenderableObject *Owner, const
	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string
	&FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
	: CComponent(Name,Owner)
	, m_FnOnCreate(FnOnCreate)
	, m_FnOnDestroy(FnOnDestroy)
	, m_FnOnUpdate(FnOnUpdate)
	, m_FnOnRender(FnOnRender)
	, m_FnOnRenderDebug(FnOnRenderDebug)
{
	CEngine::GetSingleton().getScriptManager()->RunCode(m_FnOnCreate);
}

CScriptedComponent::~CScriptedComponent()
{
	CEngine::GetSingleton().getScriptManager()->RunCode(m_FnOnDestroy);
}


void CScriptedComponent::Update(float ElapsedTime)
{
	CEngine::GetSingleton().getScriptManager()->RunCode(m_FnOnUpdate);
}

void CScriptedComponent::Render(CContextManager  &_context)
{
	CEngine::GetSingleton().getScriptManager()->RunCode(m_FnOnRender);
}

void CScriptedComponent::RenderDebug(CContextManager  &_context)
{
	CEngine::GetSingleton().getScriptManager()->RunCode(m_FnOnRenderDebug);
}

