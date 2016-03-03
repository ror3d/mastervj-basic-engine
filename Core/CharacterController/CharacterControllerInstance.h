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
	
private:

	CFPSCameraController * m_fpsCamera;
	CRenderableObject * m_objectModel;
	bool m_activeMovAnim;

	bool Move(float _elapsedTime);
	void AnimateModel(bool inMovement);

};

#endif