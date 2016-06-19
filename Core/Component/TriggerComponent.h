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
	CTriggerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	CTriggerComponent(const std::string& name, CElement* Owner);
	virtual ~CTriggerComponent();

	virtual void FixedUpdate( float ElapsedTime );

	virtual void Destroy();

};

#endif
