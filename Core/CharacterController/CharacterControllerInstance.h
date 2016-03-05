#ifndef CHARACTER_CONTROLLER_INSTANCE_H
#define CHARACTER_CONTROLLER_INSTANCE_H

#include <Utils/Named.h>

class CFPSCameraController;
class CRenderableObject;

class CCharacterControllerInstance : public CNamed
{

public:
	CCharacterControllerInstance(std::string Name, CFPSCameraController * fpsCam, CRenderableObject * rendObjec);
	virtual ~CCharacterControllerInstance();

	void Update(float _elapsedTime);
	CFPSCameraController * getFpsCamera(){ return m_fpsCamera;  }
	CRenderableObject * getObjectModel(){ return m_objectModel;  }
	
private:

	CFPSCameraController * m_fpsCamera;
	CRenderableObject * m_objectModel;

	


};

#endif