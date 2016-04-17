#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include "Utils/TMapManager.h"


class CTrigger;
class CXMLTreeNode;

class CTriggerManager : public TMapManager<CTrigger>
{
public:
	CTriggerManager();
	virtual ~CTriggerManager();
	bool Load(std::string fileName);
	void Activate(std::string nameTrigger);
private:
	std::string m_FileName;

};

#endif
