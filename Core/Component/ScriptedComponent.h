#ifndef SCRIPTED_COMPONENT_H
#define SCRIPTED_COMPONENT_H

#include "Component.h"

#include <vector>

class CScriptManager;

class CScriptedComponent : public CComponent
{
private:
	CScriptManager* m_scriptMgr;

	unsigned m_componentStateId;

	static unsigned s_nextComponentStateId;

	void SetComponent();

	static std::vector<CScriptedComponent*> s_components;
public:
	CScriptedComponent(const std::string& Name, CRenderableObject* Owner);
	virtual ~CScriptedComponent();
	void Init();
	virtual void Update(float ElapsedTime);
	virtual void Render(CContextManager&  _context);
	virtual void RenderDebug(CContextManager&  _context);

	virtual void Destroy();

	static void InitAll();
};

#endif
