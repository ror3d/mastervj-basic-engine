#ifndef CHARACTER_CONTROLLER_COMPONENT_H
#define CHARACTER_CONTROLLER_COMPONENT_H

#include "Component.h"

#include <Base/Math/Math.h>

class CCharacterControllerComponent : public CComponent
{
	float m_height;
	float m_radius;
	float m_density;
	Vect3f m_offset;

	bool m_stickToGround;

protected:
	virtual void Init();

public:
	CCharacterControllerComponent(CXMLTreeNode& node, CElement* Owner);
	CCharacterControllerComponent(const CCharacterControllerComponent& base, CElement* Owner);
	virtual ~CCharacterControllerComponent();

	virtual void FixedUpdate( double ElapsedTime );

	virtual void Destroy();

	bool IsGrounded();

	Vect3f Move(const Vect3f& displacement, const Vect3f& up, float elapsedTime);

	void SetPosition(const Vect3f& pos);

	void Resize( float height, float radius );

	float GetHeight();
	float GetRadius();


	inline bool GetStickToGround() const { return m_stickToGround; }
	inline void SetStickToGround(bool s) { m_stickToGround = s; }

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CCharacterControllerComponent( *this, Owner ); }
};

#endif
