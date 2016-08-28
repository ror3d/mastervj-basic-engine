#include "CharControllerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

const std::string CCharacterControllerComponent::COMPONENT_TYPE = "CharacterController";

CCharacterControllerComponent::CCharacterControllerComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_height = node.GetFloatProperty( "height", 1, false );
	m_radius = node.GetFloatProperty( "radius", 0.5, false );
	m_density = node.GetFloatProperty( "density", 1, false );
	m_offset = node.GetVect3fProperty( "offset", Vect3f(0, 0, 0), false );
}

CCharacterControllerComponent::CCharacterControllerComponent(const CCharacterControllerComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_height = base.m_height;
	m_radius = base.m_radius;
	m_density = base.m_density;
	m_offset = base.m_offset;
}

CCharacterControllerComponent::~CCharacterControllerComponent()
{}

void CCharacterControllerComponent::Init()
{
	CEngine::GetSingleton().getPhysXManager()->createController(m_height, m_radius, m_density, GetOwner()->GetPosition() - m_offset, getName());
}

void CCharacterControllerComponent::Destroy()
{
	CEngine::GetSingleton().getPhysXManager()->releaseCharacterController( getName() );
}


void CCharacterControllerComponent::FixedUpdate(float ElapsedTime)
{
}

bool CCharacterControllerComponent::IsGrounded()
{
	return CEngine::GetSingleton().getPhysXManager()->isCharacterControllerGrounded( getName() );
}

void CCharacterControllerComponent::Move(const Vect3f& displacement, const Vect3f& up, float elapsedTime)
{
	if(displacement.SquaredLength() < 0.00001 )
	{
		return;
	}
	GetOwner()->SetPosition(CEngine::GetSingleton().getPhysXManager()->moveCharacterController( displacement, up, elapsedTime, getName() ) + m_offset);
}

void CCharacterControllerComponent::SetPosition(const Vect3f& pos)
{
	CEngine::GetSingleton().getPhysXManager()->setPosition(pos, getName());
}

void CCharacterControllerComponent::Resize( float height, float radius )
{
	CEngine::GetSingleton().getPhysXManager()->resizeCharacterController(getName(), height, radius);
	float center = m_height / 2 + m_radius;
	float nCenter = height / 2 + radius;
	m_height = height;
	m_radius = radius;
}

float CCharacterControllerComponent::GetHeight()
{
	return m_height;
}

float CCharacterControllerComponent::GetRadius()
{
	return m_radius;
}