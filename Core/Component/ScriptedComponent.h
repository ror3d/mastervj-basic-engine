#ifndef SCRIPTED_COMPONENT_H
#define SCRIPTED_COMPONENT_H

#include "Component.h"

class CScriptedComponent : public CComponent
{
private:
	std::string m_FnOnCreate;
	std::string m_FnOnDestroy;
	std::string m_FnOnUpdate;
	std::string m_FnOnRender;
	std::string m_FnOnRenderDebug;
public:
	CScriptedComponent(const std::string &Name, CRenderableObject *Owner, const
	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string
	&FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug);
	virtual ~CScriptedComponent();
	virtual void Update(float ElapsedTime);
	virtual void Render(CContextManager  &_context);
	virtual void RenderDebug(CContextManager  &_context);
};

#endif
