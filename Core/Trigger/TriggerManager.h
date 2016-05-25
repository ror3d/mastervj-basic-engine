#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include "Utils/TMapManager.h"

#include <vector>

class CTriggerComponent;
class CXMLTreeNode;

class CTriggerManager : public TMapManager<CTriggerComponent>
{
public:

	CTriggerManager();
	virtual ~CTriggerManager();
	void AddElement(CTriggerComponent *comp);
	void Activate(std::string nameTrigger);
	void Deactivate(std::string nameTrigger);
private:
	std::string m_FileName;
	std::vector<std::string> activedTriggers;
};

#endif
