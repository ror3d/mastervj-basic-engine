#ifndef CHARACTER_CONTROLLER_MANAGER_H
#define CHARACTER_CONTROLLER_MANAGER_H

#include <Utils/TMapManager.h>

class CCharacterControllerInstance;

class CCharacterControllerManager : public TMapManager<CCharacterControllerInstance>
{
public:
	CCharacterControllerManager();
	virtual ~CCharacterControllerManager();

	void UpdateInstances(float _elapsedTime);
	void Create(std::string Name, std::string CameraName, std::string layerName, std::string modelName);
	
};

#endif