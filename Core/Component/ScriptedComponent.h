#ifndef SCRIPTED_COMPONENT_H
#define SCRIPTED_COMPONENT_H

#include "Component.h"

#include <vector>

class CScriptManager;
class CXMLTreeNode;

class CScriptedComponent : public CComponent
{
private:
	CScriptManager* m_scriptMgr;

	unsigned m_componentStateId;

	static unsigned s_nextComponentStateId;

	void SetComponent();

protected:
	virtual void Init();

public:
	CScriptedComponent(const std::string& name, CRenderableObject* Owner);
	CScriptedComponent(CXMLTreeNode& node, CRenderableObject* Owner);
	virtual ~CScriptedComponent();
	virtual void Update(float ElapsedTime);
	virtual void FixedUpdate(float ElapsedTime);
	virtual void Render(CContextManager&  _context);
	virtual void RenderDebug(CContextManager&  _context);

	virtual void SendMsg(const std::string msg);

	virtual void Destroy();
};

#endif
