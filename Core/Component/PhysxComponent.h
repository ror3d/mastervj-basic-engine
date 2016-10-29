#ifndef PHYSX_COMPONENT_H
#define PHYSX_COMPONENT_H

#include "Component.h"

#include <Base/Math/Math.h>
#include <vector>

class CPhysxComponent : public CComponent
{

protected:
	std::string m_colType;
	std::string m_coreName;
	bool m_isStatic;
	bool m_isKinematic;
	bool m_isTrigger;
	Vect3f m_scaleOffset;
	Vect3f m_positionOffset;
	float m_mass;

	virtual void Init();

	void Init(Vect3f scale, Vect3f position);
public:
	CPhysxComponent(CXMLTreeNode& node, CElement* Owner);
	CPhysxComponent(const CPhysxComponent& base, CElement* Owner);
	virtual ~CPhysxComponent();

	virtual void PhysxUpdate();
	virtual void FixedUpdate(double ElapsedTime);

	virtual void Destroy();

	void Recreate();

	void Move(Vect3f position);

	std::string GetColType(){ return m_colType; }
	bool IsStatic(){ return m_isStatic;  }
	bool IsKinematic(){ return m_isKinematic;  }

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CPhysxComponent( *this, Owner ); }
};

#endif
