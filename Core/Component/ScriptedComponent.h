#ifndef SCRIPTED_COMPONENT_H
#define SCRIPTED_COMPONENT_H

#include "Component.h"

#include <vector>

class CScriptManager;
class CXMLTreeNode;

class CScriptedComponent : public CComponent
{
private:
	std::string m_scriptClass;

	CScriptManager* m_scriptMgr;

	unsigned m_componentStateId;

	static unsigned s_nextComponentStateId;

	void SetComponent();

protected:
	virtual void Init();

	virtual void OnObjectInitialized();

public:
	CScriptedComponent(const CScriptedComponent& base, CElement* Owner);
	CScriptedComponent(CXMLTreeNode& node, CElement* Owner);
	virtual ~CScriptedComponent();

	virtual void Update(float ElapsedTime);
	virtual void FixedUpdate(float ElapsedTime);
	virtual void Render(CContextManager&  _context);
	virtual void RenderDebug(CContextManager&  _context);

	virtual void SendMsg(const std::string &msg);
	virtual void SendMsg(const std::string &msg, int arg1);
	virtual void SendMsg(const std::string &msg, const std::string &arg1);
	virtual void SendMsg(const std::string &msg, CElement* arg1);

	virtual void Destroy();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CScriptedComponent( *this, Owner ); }
};

#endif
