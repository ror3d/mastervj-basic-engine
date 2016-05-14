#ifndef CHARACTER_CONTROLLER_COMPONENT_H
#define CHARACTER_CONTROLLER_COMPONENT_H

#include "Component.h"

#include <vector>

class CCharacterControllerComponent : public CComponent
{
public:
	CCharacterControllerComponent(CXMLTreeNode& node, CRenderableObject* Owner);
	CCharacterControllerComponent(CRenderableObject* Owner);
	virtual ~CCharacterControllerComponent();
	void Init();

	virtual void Update(float ElapsedTime);

	virtual void Destroy();

	bool IsGrounded();

	void Move(const Vect3f& position, const Vect3f& up, float elapsedTime);
};

#endif
