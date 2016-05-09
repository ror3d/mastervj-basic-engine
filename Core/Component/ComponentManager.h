#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

#include "Component.h"
#include <set>
#include <Base/Utils/TMapManager.h>

class CContextManager;

class CComponentManager
{
private:
	std::set<CComponent*> m_components;
public:
	CComponentManager();
	virtual ~CComponentManager();
	void Update(float ElapsedTime);
	void Render(CContextManager  &_context);
	void RenderDebug(CContextManager  &_context);
	bool AddComponent(CComponent *Component);
	void destroy();
}; 

#endif
