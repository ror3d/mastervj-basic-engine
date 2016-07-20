#ifndef FPS_CAMERA_COMPONENT_H
#define FPS_CAMERA_COMPONENT_H

#include "Component.h"

#include <Base/Math/Math.h>

class CFPSCameraComponent : public CComponent
{
	Vect3f m_CamOffset;
	Vect3f m_CamDisplacement;
	bool m_followRenderableObject;
	float m_characterRotationOverride;

protected:
	virtual void Init();

public:
	CFPSCameraComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	CFPSCameraComponent(const std::string& name, CElement* Owner);
	virtual ~CFPSCameraComponent();

	virtual void Update( float elapsedTime );

	virtual void Destroy();

	void SetAsCurrentCamera();

	void SetFollowCharacter(bool follow, float overrideRot, float camOffset, float camDisplacement);

	float GetYaw();
};

#endif
