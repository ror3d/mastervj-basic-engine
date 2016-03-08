#include "ScriptedComponent.h"

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
}

void CScriptedComponent::Update(float ElapsedTime)
{
	/*try
	{
		if (!m_FnOnUpdate.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager().GetLuaState(),
			m_FnOnUpdate.c_str(), m_Owner, ElapsedTime);
	}
	catch (const luabind::error &e)
	{
		ShowLuabindError(e);
	}*/
	//LE PASAMOS PARAM!!
}

void CScriptedComponent::Render(CContextManager  &_context)
{

}

void CScriptedComponent::RenderDebug(CContextManager  &_context)
{

}

CScriptedComponent * CScriptedComponent::CreateScriptedComponent(const std::string &Name,
	CRenderableObject *Owner, const std::string &FnOnCreate, const std::string
	&FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const
	std::string &FnOnRenderDebug)
{
	return new CScriptedComponent(Name, Owner, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug);
}