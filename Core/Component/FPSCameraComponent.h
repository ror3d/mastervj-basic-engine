#ifndef FPS_CAMERA_COMPONENT_H
#define FPS_CAMERA_COMPONENT_H

#include "Component.h"

#include <Base/Math/Math.h>

class CFPSCameraComponent : public CComponent
{
	Vect3f m_CamOffset;

	float m_YawSpeed;
	float m_PitchSpeed;
	float m_PitchFloorLimit;
	float m_PitchSkyLimit;
protected:
	virtual void Init();

public:
	CFPSCameraComponent(CXMLTreeNode& node, CElement* Owner);
	CFPSCameraComponent(const CFPSCameraComponent& base, CElement* Owner);
	virtual ~CFPSCameraComponent();

	virtual void Update( float elapsedTime );

	virtual void Destroy();

	void Reset();

	void SetAsCurrentCamera();

	Vect3f GetCamOffset() { return m_CamOffset; }
	void SetCamOffset( Vect3f offset ) { m_CamOffset = offset; }

	float GetYaw();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CFPSCameraComponent( *this, Owner ); }
};

#endif
