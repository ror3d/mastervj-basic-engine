#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

#include "Component.h"
#include <set>
#include <map>
#include <vector>
#include <string>
#include <Base/Utils/TMapManager.h>

class CContextManager;

class CComponentManager
{
private:
	static std::vector<const char*> s_componentsUpdateOrder;

	std::map<std::string, CComponent*> m_componentsMap;
	std::set<CComponent*> m_components;
	std::map<std::string, std::set<CComponent*>> m_componentsByType;

	std::vector<CComponent*> m_componentsToAdd;
	std::vector<CComponent*> m_componentsToRemove;
	bool m_initialized;
public:
	CComponentManager();
	virtual ~CComponentManager();
	void PhysxUpdate();
	void Update(float ElapsedTime);
	void FixedUpdate(float ElapsedTime);
	void Render(CContextManager  &_context);
	void RenderDebug(CContextManager  &_context);
	bool AddComponent(CComponent *Component);
	bool RemoveComponent(CComponent *Component);
	void FirstInitialization();
	CComponent* get(const std::string& componentName) const;
	void destroy();

	inline bool IsInitialized() const { return m_initialized; }

private:
	void DestroyRemovedComponents();
};

#endif
