#ifndef SPAWN_COMPONENT_H
#define SPAWN_COMPONENT_H

#include "Component.h"


class CSpawnComponent : public CComponent
{


protected:
	virtual void Init();

public:
	CSpawnComponent(CXMLTreeNode& node, CElement* Owner);
	CSpawnComponent(const CSpawnComponent& base, CElement* Owner);
	virtual ~CSpawnComponent();
	
	virtual void FixedUpdate( float ElapsedTime );

	virtual void Destroy();

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CSpawnComponent(*this, Owner); }
};

#endif
