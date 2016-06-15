#ifndef PHYSX_COMPONENT_H
#define PHYSX_COMPONENT_H

#include "Component.h"

#include <vector>

class CPhysxComponent : public CComponent
{
	
private: 
	std::string m_colType;
	std::string m_coreName;
	bool m_isStatic;
	bool m_isKinematic;
	bool m_isTrigger;

protected:
	virtual void Init();

public:
	CPhysxComponent(CXMLTreeNode& node, CRenderableObject* Owner, std::string nameCore);
	CPhysxComponent(CRenderableObject* Owner);
	virtual ~CPhysxComponent();

	virtual void FixedUpdate(float ElapsedTime);

	virtual void Destroy();

	void Move();

	std::string GetColType(){ return m_colType; }
	bool IsStatic(){ return m_isStatic;  }
	bool IsKinematic(){ return m_isKinematic;  }

};

#endif
