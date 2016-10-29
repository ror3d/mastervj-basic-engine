#ifndef CINEMATIC_COMPONENT_H
#define CINEMATIC_COMPONENT_H

#include "Component.h"


class CCinematicComponent : public CComponent
{


protected:
	virtual void Init();

public:
	CCinematicComponent(CXMLTreeNode& node, CElement* Owner);
	CCinematicComponent(const CCinematicComponent& base, CElement* Owner);
	virtual ~CCinematicComponent();

	virtual void FixedUpdate( double ElapsedTime );

	virtual void Destroy();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CCinematicComponent(*this, Owner); }
};

#endif
