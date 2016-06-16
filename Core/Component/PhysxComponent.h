#ifndef PHYSX_COMPONENT_H
#define PHYSX_COMPONENT_H

#include "Component.h"

#include <vector>

class CPhysxComponent : public CComponent
{
	
protected:
	std::string m_colType;
	std::string m_coreName;
	bool m_isStatic;
	bool m_isKinematic;
	bool m_isTrigger;

	virtual void Init();

	void Init(Vect3f scale, Vect3f position);
public:
	CPhysxComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	CPhysxComponent(const std::string& name, CElement* Owner);
	virtual ~CPhysxComponent();

	virtual void FixedUpdate(float ElapsedTime);

	virtual void Destroy();

	void Move(Vect3f position);

	std::string GetColType(){ return m_colType; }
	bool IsStatic(){ return m_isStatic;  }
	bool IsKinematic(){ return m_isKinematic;  }

};

#endif
