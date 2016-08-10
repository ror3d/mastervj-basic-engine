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
	bool m_isCameraLocked;

protected:
	virtual void Init();

public:
	CFPSCameraComponent(CXMLTreeNode& node, CElement* Owner);
	CFPSCameraComponent(const CFPSCameraComponent& base, CElement* Owner);
	virtual ~CFPSCameraComponent();

	virtual void Update( float elapsedTime );

	virtual void Destroy();

	void SetAsCurrentCamera();

	void SetFollowCharacter(bool follow, float overrideRot, float camOffset, float camDisplacement, bool camLocked);

	float GetYaw();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CFPSCameraComponent( *this, Owner ); }
};

#endif
