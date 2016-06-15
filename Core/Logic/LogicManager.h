#ifndef LOGIC_MANAGER_H
#define LOGIC_MANAGER_H

#include "Utils/TMapManager.h"
#include <vector>

class CLogicManager
{
public:
	CLogicManager();
	virtual ~CLogicManager();

	std::string m_FileName;	
	std::vector<std::string> *  m_activedLogic;

	void Update();
	void LoadXML(std::string fileName);
	std::string GetActivableObject();
	void RemoveActivedLogic(std::string nameToRemove);

	void destroy() {}
};

#endif
