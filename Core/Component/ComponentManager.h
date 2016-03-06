#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

#include "Utils/TMapManager.h"
#include "Component.h"

class CContextManager;

class CComponentManager : public TMapManager<CComponent>
{
public:
	CComponentManager();
	virtual ~CComponentManager();
	void Update(float ElapsedTime);
	void Render(CContextManager  &_context);
	void RenderDebug(CContextManager  &_context);
	bool AddComponent(const std::string &Name, CComponent *Component);
}; 

#endif
