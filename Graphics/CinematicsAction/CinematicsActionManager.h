#ifndef CINEMATICS_ACTION_MANAGER_H
#define CINEMATICS_ACTION_MANAGER_H

#include "Utils/TMapManager.h"

class CCinematicsAction;

class CCinematicsActionManager
{
public:
	CCinematicsActionManager();
	virtual ~CCinematicsActionManager();

	std::string m_FileName;	

	void Update();
	void LoadXML(std::string fileName);
};

#endif