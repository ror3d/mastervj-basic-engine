#ifndef FREE_CAMERA_COMPONENT_H
#define FREE_CAMERA_COMPONENT_H

#include "Component.h"

#include <Base/Math/Math.h>

class CFreeCameraController;

class CFreeCameraComponent : public CComponent
{
protected:
	virtual void Init();

	CFreeCameraController* m_camController;

public:
	CFreeCameraComponent(CXMLTreeNode& node, CElement* Owner);
	CFreeCameraComponent(const CFreeCameraComponent& base, CElement* Owner);
	virtual ~CFreeCameraComponent();

	virtual void Update( float elapsedTime );

	virtual void Destroy();

	void Reset();

	void SetAsCurrentCamera();

	Vect3f GetOffset();
	void SetOffset( Vect3f offset );

	Vect3f GetForward();
	void SetForward( Vect3f fwd );

	Vect3f GetUp();
	void SetUp( Vect3f up );


	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CFreeCameraComponent( *this, Owner ); }
};

#endif
