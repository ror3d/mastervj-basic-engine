#ifndef TRIGGER_COMPONENT_H
#define TRIGGER_COMPONENT_H

#include "PhysxComponent.h"

#include <vector>

class CTriggerComponent : public CPhysxComponent
{
	Vect3f m_scale;
	Vect3f m_offset;

	std::vector<std::string> m_activeElements;

protected:
	virtual void Init();

public:
	CTriggerComponent(CXMLTreeNode& node, CElement* Owner);
	CTriggerComponent(const CTriggerComponent& base, CElement* Owner);
	virtual ~CTriggerComponent();

	virtual void PhysxUpdate();
	virtual void FixedUpdate( double ElapsedTime );

	virtual void Destroy();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CTriggerComponent( *this, Owner ); }
};

#endif
