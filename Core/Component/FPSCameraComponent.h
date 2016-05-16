#ifndef FPS_CAMERA_COMPONENT_H
#define FPS_CAMERA_COMPONENT_H

#include "Component.h"

#include <vector>

class CFPSCameraComponent : public CComponent
{
	Vect3f m_CamOffset;

protected:
	virtual void Init();

public:
	CFPSCameraComponent(CXMLTreeNode& node, CRenderableObject* Owner);
	CFPSCameraComponent(CRenderableObject* Owner);
	virtual ~CFPSCameraComponent();

	virtual void FixedUpdate( float elapsedTime );

	virtual void Destroy();
};

#endif
